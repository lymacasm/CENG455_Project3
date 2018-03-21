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
	_mqx_uint list_size = 0;
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
	_mqx_uint min_task_priority;
	_partition_id task_list_pid;

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
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create response message pool */
	rsp_msg_pool = _msgpool_create(sizeof(SCHEDULER_RESPONSE_MSG), RSP_POOL_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open response message pool.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create partition for task queue elements */
	task_list_pid = _partition_create(sizeof(SCH_TASK_NODE), TASK_LIST_INITIAL_SIZE,
			TASK_LIST_GROW_SIZE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to create partition.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Get absolute lowest task priority */
	min_task_priority = _sched_get_min_priority(0);

#ifdef PEX_USE_RTOS
	while (1) {
#endif
		time_t timeout = 0;
		time_t abs_deadline = 0;
		if(!_queue_is_empty(&active_list))
		{
			SCH_TASK_NODE_PTR active_list_itr;

			/* Get the soonest deadline */
			//active_list_itr = (SCH_TASK_NODE_PTR)_queue_head
		}

		/* Block until the next deadline or until a message is received */
		request_msg = (SCHEDULER_REQUEST_MSG_PTR)_msgq_receive(msg_qid, timeout);

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
				response_msg->ACK_ID = CREATE_ACK;
				SCH_TASK_NODE_PTR new_task;

				/* Create a task. It won't start until the scheduler sleeps */
				_task_create(0, request_msg->TASK_INFO->task_type, 0);

				/* Allocate memory for new task */
				new_task = (SCH_TASK_NODE_PTR)_partition_alloc(task_list_pid);

				/* Initialize members of new_task */
				new_task->CREATION_TIME = request_msg->TASK_INFO->creation_time;
				new_task->TASK_TYPE = request_msg->TASK_INFO->task_type;
				new_task->ABS_DEADLINE = new_task->CREATION_TIME +
						request_msg->TASK_INFO->deadline;
				new_task->TID = request_msg->TASK_INFO->tid;

				/* Add the task into the list */
				queue_insert_task(&active_list, new_task);

				/* Adjust the task priorities */
				queue_prioritize_tasks(&active_list);
				break;

			/* Delete a task */
			case(DELETE):
				response_msg->ACK_ID = DELETE_ACK;
				break;
			case(ACTIVE_LIST):
				response_msg->ACK_ID = ACTIVE_LIST_ACK;
				break;
			case(OVRDUE_LIST):
				response_msg->ACK_ID = OVRDUE_LIST_ACK;
				break;
			case(RESET):
				response_msg->ACK_ID = RESET_ACK;
				break;
			default:
				response_msg->ACK_ID = SCH_UNKNOWN_ACK;
				break;
			}

			/* Send the response */
			_msgq_send(response_msg);

			/* Free the request message data */
			_msg_free(request_msg);
		}
		/* Task deadline elapsed. Kill task. */
		else
		{
			/* Move task to overdue list */
			/* Terminate task */
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
