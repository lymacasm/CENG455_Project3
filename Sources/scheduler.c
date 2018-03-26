/* ###################################################################
**     Filename    : scheduler.c
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-13, 15:50, # CodeGen: 9
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         scheduler_task - void scheduler_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file scheduler.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup scheduler_module scheduler module documentation
**  @{
*/         
/* MODULE scheduler */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "user_task.h"
#include "scheduler.h"
#include "periodic_task_gen.h"
#include "monitor_task.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

#include <stdio.h>
#include <partition.h>

/* Used for message memory pools */
#define REQ_POOL_SIZE 8
#define RSP_POOL_SIZE REQ_POOL_SIZE

/* Defines the highest possible task priority */
#define HIGHEST_PRIORITY 15

/* Partition definitions */
#define TASK_LIST_INITIAL_SIZE (8)
#define TASK_LIST_GROW_SIZE (4)

_pool_id req_msg_pool;
_pool_id rsp_msg_pool;


/* Inserts the task into a sorted queue, in O(N) runtime */
static void queue_insert_task(QUEUE_STRUCT_PTR list, SCH_TASK_NODE_PTR task)
{
	SCH_TASK_NODE_PTR list_prev_node;
	SCH_TASK_NODE_PTR list_curr_node;

	/* If the list is empty, simply add task to list */
	if(_queue_is_empty(list))
	{
		_queue_enqueue(list, (QUEUE_ELEMENT_STRUCT_PTR)task);
		return;
	}

	/* Search list, starting from the beginning */
	list_prev_node = NULL;
	list_curr_node = (SCH_TASK_NODE_PTR)_queue_head(list);

	/* Loop through the list */
	while(list_curr_node != NULL)
	{
		/* Condition for placing task before current list item */
		if(task->ABS_DEADLINE < list_curr_node->ABS_DEADLINE) break;

		/* Increment list node pointers */
		list_prev_node = list_curr_node;
		list_curr_node = (SCH_TASK_NODE_PTR)_queue_next(list,
				(QUEUE_ELEMENT_STRUCT_PTR)list_curr_node);
	}

	/* Insert task into list at previously determined location */
	_queue_insert(list, (QUEUE_ELEMENT_STRUCT_PTR)list_prev_node,
			(QUEUE_ELEMENT_STRUCT_PTR)task);
}

/* This function adjusts the priority of the tasks linearly in O(N) runtime */
static void queue_prioritize_tasks(QUEUE_STRUCT_PTR list)
{
	SCH_TASK_NODE_PTR list_itr;
	_mqx_uint priority = HIGHEST_PRIORITY;

	/* Get the beginning of the list, and the size */
	list_itr = (SCH_TASK_NODE_PTR)_queue_head(list);

	/* Iterate through the list */
	while(list_itr != NULL)
	{
		_mqx_uint old_priority; /* Unused variable needed for _task_set_priority */
		_mqx_uint err = MQX_OK;

		/* Adjust the priority of the task */
		err = _task_set_priority(list_itr->TID, priority, &old_priority);
		if(err != MQX_OK)
		{
			printf("Failed to set task priority.\n");
			printf("TID: %lu, Priority: %lu\n", list_itr->TID, priority);
			return;
		}

		/* Update priority member */
		list_itr->TASK_PRIORITY = priority;

		/* Lower the priority for the next task in the list */
		priority++;

		/* Increment list iterator pointer */
		list_itr = (SCH_TASK_NODE_PTR)_queue_next(list,
				(QUEUE_ELEMENT_STRUCT_PTR)list_itr);
	}
}

/* Searches queue for element to remove.
 * Returns True if element if found and removed.
 * Returns False if the element if neither found nor removed.
 */
static uint8_t queue_find_and_remove(QUEUE_STRUCT_PTR list, _task_id tid)
{
	SCH_TASK_NODE_PTR list_itr;

	/* Start at head of list */
	list_itr = (SCH_TASK_NODE_PTR)_queue_head(list);

	while(list_itr != NULL)
	{
		/* Check for task id */
		if(list_itr->TID == tid)
		{
			/* Remove item from the queue */
			_queue_unlink(list, (QUEUE_ELEMENT_STRUCT_PTR)list_itr);

			/* Free the memory */
			_partition_free(list_itr);

			/* Return TRUE for a successful operation */
			return TRUE;
		}

		/* Increment list iterator pointer */
		list_itr = (SCH_TASK_NODE_PTR)_queue_next(list,
				(QUEUE_ELEMENT_STRUCT_PTR)list_itr);
	}

	return FALSE;
}

static uint8_t queue_find(QUEUE_STRUCT_PTR list, _task_id tid)
{
	SCH_TASK_NODE_PTR list_itr;

	/* Start at head of list */
	list_itr = (SCH_TASK_NODE_PTR)_queue_head(list);

	while(list_itr != NULL)
	{
		/* Check for task id */
		if(list_itr->TID == tid)
		{
			/* Return TRUE for a successful operation */
			return TRUE;
		}

		/* Increment list iterator pointer */
		list_itr = (SCH_TASK_NODE_PTR)_queue_next(list,
				(QUEUE_ELEMENT_STRUCT_PTR)list_itr);
	}

	return FALSE;
}

static void queue_remove_all(QUEUE_STRUCT_PTR list, const uint8_t delete_option)
{
	SCH_TASK_NODE_PTR list_itr;
	uint32_t count = _queue_get_size(list);

	/* Delete all elements in queue */
	while(count > 0)
	{
		/* Remove first element in queue */
		list_itr = (SCH_TASK_NODE_PTR)_queue_dequeue(list);

		/* Destroy task */
		if(delete_option)
		{
			_task_destroy(list_itr->TID);
		}

		/* Free the memory for that element */
		_partition_free(list_itr);

		/* Decrement counter */
		count--;
	}
}

/*
** ===================================================================
**     Callback    : scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void scheduler_task(os_task_param_t task_init_data)
{
	/* Write your local variable definition here */
	QUEUE_STRUCT active_list;
	QUEUE_STRUCT overdue_list;
	SCHEDULER_REQUEST_MSG_PTR request_msg;
	SCHEDULER_RESPONSE_MSG_PTR response_msg;
	_queue_id msg_qid;
	_partition_id task_list_pid;
	MQX_TICK_STRUCT current_t;
	/* Overhead timers */
	time_t start_time = 0;
	time_t end_time = 0;
	time_t temp_overhead = 0;
	time_t overhead = 0;

	printf("\n\nScheduler created!\n\n");

	/* Initialize queues */
	_queue_init(&active_list, 0);
	_queue_init(&overdue_list, 0);

	/* Open the message queue */
	msg_qid = _msgq_open(SCHEDULER_QUEUE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open Scheduler message queue.\n");
		printf("Error code: %x\n", MQX_OK);
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create request message pool */
	req_msg_pool = _msgpool_create(sizeof(SCHEDULER_REQUEST_MSG), REQ_POOL_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open request message pool.\n");
		printf("Error code: %x\n", (unsigned int)_task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create response message pool */
	rsp_msg_pool = _msgpool_create(sizeof(SCHEDULER_RESPONSE_MSG), RSP_POOL_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open response message pool.\n");
		printf("Error code: %x\n", (unsigned int)_task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create partition for task queue elements */
	task_list_pid = _partition_create(sizeof(SCH_TASK_NODE), TASK_LIST_INITIAL_SIZE,
			TASK_LIST_GROW_SIZE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to create partition.\n");
		printf("Error code: %x\n", (unsigned int)_task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

#ifdef PEX_USE_RTOS
	while (1) {
#endif
		time_t timeout = 0;
		if(!_queue_is_empty(&active_list))
		{
			SCH_TASK_NODE_PTR first_active_task;
			MQX_TICK_STRUCT current_time;
			time_t abs_deadline;

			/* Get the task with the soonest deadline */
			first_active_task = (SCH_TASK_NODE_PTR)_queue_head(&active_list);

			/* Get the soonest deadline */
			abs_deadline = first_active_task->ABS_DEADLINE;

			/* Get the current time */
			_time_get_elapsed_ticks(&current_time);

			/* Get the timeout */
			timeout = abs_deadline - current_time.TICKS[0];

			//printf("Abs Deadline:0x%x, Current Time:0x%x\n", abs_deadline, current_time.TICKS[0]);
		}

		/* Calculate Scheduler Overhead*/
		_time_get_ticks(&current_t);
		end_time = current_t.TICKS[0];
		if (end_time > start_time){
			temp_overhead = end_time - start_time;
		}
		overhead += temp_overhead;

		//printf("Sleeping for %x ticks\n", (unsigned int)timeout);

		/* Block until the next deadline or until a message is received */
		request_msg = (SCHEDULER_REQUEST_MSG_PTR)_msgq_receive(msg_qid, timeout);

		/* Start time for scheduler in ticks */
		_time_get_ticks(&current_t);
		start_time = current_t.TICKS[0];

		/* Message was received. Process Message. */
		if(request_msg != NULL)
		{
			/* Allocate response message */
			response_msg = (SCHEDULER_RESPONSE_MSG_PTR)_msg_alloc(rsp_msg_pool);

			/* Set the source and target qid's */
			response_msg->HEADER.SOURCE_QID = request_msg->HEADER.TARGET_QID;
			response_msg->HEADER.TARGET_QID = request_msg->HEADER.SOURCE_QID;

			/* Check what command was received */
			switch(request_msg->CMD_ID)
			{
			/* Schedule a new task */
			case(CREATE):
				//printf("Creating task\n");
				response_msg->ACK_ID = CREATE_ACK;
				SCH_TASK_NODE_PTR new_task;
				_task_id new_task_tid;

				/* Create a task. It won't start until the scheduler sleeps */
				new_task_tid = _task_create(0, request_msg->TASK_INFO->task_type,
						request_msg->PARAMETER);

				/* Allocate memory for new task */
				new_task = (SCH_TASK_NODE_PTR)_partition_alloc(task_list_pid);

				/* Initialize members of new_task */
				new_task->CREATION_TIME = request_msg->TASK_INFO->creation_time;
				new_task->TASK_TYPE = request_msg->TASK_INFO->task_type;
				new_task->ABS_DEADLINE = request_msg->TASK_INFO->creation_time +
						request_msg->TASK_INFO->deadline;
				new_task->TID = new_task_tid;

				/* Add the task into the list */
				queue_insert_task(&active_list, new_task);

				/* Adjust the task priorities */
				queue_prioritize_tasks(&active_list);

				/* Populate task id */
				response_msg->TID = new_task_tid;

				/* Set status */
				response_msg->STATUS = SUCCESSFUL;
				//printf("Created task %x!\n", (unsigned int)new_task_tid);
				break;

			/* Delete a task */
			case(DELETE):
				response_msg->ACK_ID = DELETE_ACK;
				//printf("Deleting task %x\n", (unsigned int)request_msg->TASK_INFO->tid);
				if(queue_find_and_remove(&active_list, request_msg->TASK_INFO->tid))
				{
					/* Tell the requester everything was good */
					response_msg->STATUS = SUCCESSFUL;
					response_msg->TID = request_msg->TASK_INFO->tid;
					//printf("Task deleted\n");
				}
				else if(queue_find(&overdue_list, request_msg->TASK_INFO->tid))
				{
					/* Tell the requester the task didn't meet its deadline and has already been deleted */
					response_msg->STATUS = OVERDUE;
					response_msg->TID = request_msg->TASK_INFO->tid;
				}
				else
				{
					/* Tell the requestor the task didn't exist */
					response_msg->STATUS = FAILED;
					response_msg->TID = 0;
				}
				break;
			case(ACTIVE_LIST):
				printf("Retrieving active list\n");
				response_msg->ACK_ID = ACTIVE_LIST_ACK;
				if(_queue_get_size(&active_list) > 0)
				{
					response_msg->STATUS = SUCCESSFUL;
					response_msg->TASK_LIST = &active_list;
					printf("List retrieved\n");
				}
				else
				{
					response_msg->STATUS = FAILED;
					response_msg->TASK_LIST = NULL;
				}
				break;
			case(OVRDUE_LIST):
				printf("Retrieving overdue list\n");
				response_msg->ACK_ID = OVRDUE_LIST_ACK;
				if(_queue_get_size(&overdue_list) > 0)
				{
					response_msg->STATUS = SUCCESSFUL;
					response_msg->TASK_LIST = &overdue_list;
					printf("List retrieved\n");
				}
				else
				{
					response_msg->STATUS = FAILED;
					response_msg->TASK_LIST = NULL;
				}
				break;
			case(OVERHEAD):
				printf("Calculating Overhead \n");
				response_msg->ACK_ID = OVERHEAD_ACK;
				if(overhead > 0)
				{
					response_msg->STATUS = SUCCESSFUL;
					response_msg->TIMER = overhead;
					printf("Overhead Sent! \n");
				}
				else
				{
					response_msg->STATUS = FAILED;
					response_msg->TIMER = NULL;
				}
				break;
			case(RESET):
				printf("Resetting the scheduler\n");
				response_msg->ACK_ID = RESET_ACK;

				/* Remove all elements from active queue, and terminate all associated tasks */
				queue_remove_all(&active_list, TRUE);

				/* Remove all elements from overdue queue, don't try and terminate tasks */
				queue_remove_all(&overdue_list, FALSE);

				break;
			default:
				printf("Unknown request received\n");
				response_msg->ACK_ID = SCH_UNKNOWN_ACK;
				break;
			}

			/* Send the response */
			_msgq_send(response_msg);

			/* Free the request message data */
			_msg_free(request_msg);
		}
		/* Task deadline elapsed. Kill task. */
		else if(timeout > 0)
		{
			SCH_TASK_NODE_PTR overdue_task;

			/* Remove overdue task from active list */
			overdue_task = (SCH_TASK_NODE_PTR)_queue_dequeue(&active_list);

			/* Add overdue task to overdue list */
			_queue_enqueue(&overdue_list, (QUEUE_ELEMENT_STRUCT_PTR)overdue_task);

			printf("Task %x did not meet its deadline\n", (unsigned int)overdue_task->TID);

			/* Terminate task */
			_task_destroy(overdue_task->TID);
		}
#ifdef PEX_USE_RTOS   
	}
#endif    
}

/* END scheduler */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
