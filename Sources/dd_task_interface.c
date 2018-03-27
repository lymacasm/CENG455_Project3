/*
 * edf_task_interface.c
 *
 *  Created on: Mar 13, 2018
 *      Author: lymacasm
 */

#include "dd_task_interface.h"
#include "scheduler.h"
#include <stdio.h>
#include <partition.h>

#define DD_INTERFACE_QUEUE 9
#define TLIST_INIT_SIZE 8
#define TLIST_GROW_SIZE 4

_partition_id task_list_partition = 0;
MUTEX_STRUCT scheduler_mutex;
MUTEX_STRUCT overhead_mutex;
volatile time_t overhead = 0;

void dd_init()
{
	task_list_partition = _partition_create(sizeof(struct task_list), TLIST_INIT_SIZE,
				TLIST_GROW_SIZE, 0);
}

_task_id dd_tcreate(uint32_t template_index, uint32_t task_param, time_t deadline){
	/*
	 This primitive, creates a deadline driven scheduled task. It follows the steps outlined below
		1. Opens a queue
		2. Creates the task specified and assigns it the minimum priority possible
		3. Composes a create_task_message and sends it to the DD-scheduler
		4. Waits for a reply at the queue it created above
		5. Once the reply is received, it obtains it
		6. Destroys the queue
		7. Returns to the invoking task
	'template_index' is the template index of the task to be created.
	'deadline' is the number of clock ticks to the task's deadline.
	It returns the task_id of the created task, or an error. The error is either an MQX task creation
	error or a DD-scheduler specific error (to be determined).
	 */

	_queue_id msg_qid;
	_task_id taskID;
	_status_sch status;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	struct task_list* new_task;
	MQX_TICK_STRUCT ticks;
	TD_STRUCT_PTR td_ptr;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return 0;
	}

	new_task = (struct task_list *)_partition_alloc(task_list_partition);
	if(_task_get_error() != MQX_OK){
		printf("Failed to allocate task memory from partition.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		return 0;
	}

	// Managing Ticks
	_time_get_elapsed_ticks(&ticks);

	new_task->deadline = deadline;
	new_task->task_type = template_index;
	new_task->creation_time = ticks.TICKS[0];


	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = CREATE;
	msg_req_ptr->TASK_INFO = new_task;
	msg_req_ptr->PARAMETER = task_param;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// Send message
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	_msgq_send(msg_req_ptr);
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	/* Grab information and free message */
	status = msg_res_ptr->STATUS;
	taskID = msg_res_ptr->TID;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	// Check Status
	if (status == FAILED){
		printf("Failed to create a new task\n");
		return 0;
	}

	/* Move tasked from blocked state to the ready state */
	td_ptr = _task_get_td(taskID);
	_int_disable();
	if(td_ptr != NULL)
	{
		_task_ready(td_ptr);
		if(_task_get_error() != MQX_OK){
			printf("Failed to schedule task.\n");
			printf("Error code: %x\n", (unsigned int)_task_get_error());
			_task_set_error(MQX_OK);
			taskID = 0;
		}
	}
	_int_enable();

	// LOCK MUTEX
	error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return taskID;

}

_task_id dd_delete(_task_id task_id){
	/*
	When the function dd_delete is called, the specified task is deleted.
	This primitive deletes the task specified. It parallels the structure of the dd_tcreate as outlined
	above.
	 */
	_queue_id msg_qid;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	_status_sch status;
	_task_id taskID;
	MQX_TICK_STRUCT ticks;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return 0;
	}

	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = DELETE;
	msg_req_ptr->TASK_INFO->tid = task_id;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// Send message
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	_msgq_send(msg_req_ptr);
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	/* Get info and free message */
	taskID = msg_res_ptr->TID;
	status = msg_res_ptr->STATUS;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	// Check Status
	if (status == FAILED){
		printf("Task not created. \n");			// modify print statement later
		return 0;
	}
	else if (status == OVERDUE){
		//printf("Task Overdue, cannot delete task. \n");		// modify print statement later
		_task_destroy(msg_res_ptr->TID);
		return 0;
	}
	else if (status == SUCCESSFUL){
		_task_destroy(msg_res_ptr->TID);
	}

	// LOCK MUTEX
	error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return taskID;
}

uint32_t dd_return_active_list(struct task_list ** list){
	/*
	 This primitive requests the DD-scheduler for the list of active tasks and returns this information
	to the requestor.
	The designer must decide whether the list should be copied and sent to the requestor, or simply a
	pointer pointing to the start of the list could suffice. Analyze the alternatives and justify your implementation
	choice.
	 */
	_queue_id msg_qid;
	_status_sch status;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	QUEUE_STRUCT_PTR		active_list;
	SCH_TASK_NODE_PTR 		active_list_itr;
	struct task_list * 		list_itr;
	MQX_TICK_STRUCT ticks;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	/* Initialize output list to NULL */
	*list = NULL;

	msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return 0;
	}

	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = ACTIVE_LIST;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// Send message
	_msgq_send(msg_req_ptr);
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	/* Grab info and free message */
	active_list = (QUEUE_STRUCT_PTR)msg_res_ptr->TASK_LIST;
	status = msg_res_ptr->STATUS;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	if(status != SUCCESSFUL)
	{
		printf("Failed to read active list\n");
		return FALSE;
	}

	// copy pointer to active list
	active_list_itr = (SCH_TASK_NODE_PTR)_queue_head(active_list);

	/* Handle first element */
	if(active_list_itr != NULL)
	{
		/* Allocate new list element */
		struct task_list* list_item;
		list_item = (struct task_list *)_partition_alloc(task_list_partition);
		if(_task_get_error() != MQX_OK){
			printf("Failed to allocate task memory from partition.\n");
			printf("Error code: %x\n", (unsigned int)_task_get_error());
			_task_set_error(MQX_OK);
			return FALSE;
		}

		/* Populate list element */
		list_item->tid	= active_list_itr->TID;
		list_item->creation_time = active_list_itr->CREATION_TIME;
		list_item->deadline = active_list_itr->ABS_DEADLINE
				- active_list_itr->CREATION_TIME;
		list_item->task_type = active_list_itr->TASK_TYPE;
		list_item->next_cell = NULL;
		list_item->previous_cell = NULL;

		/* Add list item to user list */
		list_itr = list_item;

		/* Go to next element */
		active_list_itr = (SCH_TASK_NODE_PTR)_queue_next(active_list, active_list_itr);
	}

	*list = list_itr;

	/* Handle all subsequent items */
	while (active_list_itr != NULL)
	{
		/* Allocate new list element */
		struct task_list* list_item;
		list_item = (struct task_list *)_partition_alloc(task_list_partition);
		if(_task_get_error() != MQX_OK){
			printf("Failed to allocate task memory from partition.\n");
			printf("Error code: %x\n", (unsigned int)_task_get_error());
			_task_set_error(MQX_OK);
			return FALSE;
		}

		/* Populate list element */
		list_item->tid	= active_list_itr->TID;
		list_item->creation_time = active_list_itr->CREATION_TIME;
		list_item->deadline = active_list_itr->ABS_DEADLINE
				- active_list_itr->CREATION_TIME;
		list_item->task_type = active_list_itr->TASK_TYPE;
		list_item->next_cell = NULL;
		list_item->previous_cell = list_itr;

		/* Add item to end of list */
		list_itr->next_cell = list_item;

		/* Move to next element */
		list_itr = list_item;
	}

	// LOCK MUTEX
	error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return TRUE;
}

uint32_t dd_return_overdue_list(struct task_list ** list){
	/*
	This primitive requests the DD-scheduler for the list of overdue tasks and returns this information
	to the requestor. Similar in structure as the dd_return_active_list primitive.
	 */
	_queue_id msg_qid;
	_status_sch status;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	QUEUE_STRUCT_PTR		ovrdue_list;
	SCH_TASK_NODE_PTR 		ovrdue_list_itr;
	struct task_list * 		list_itr;
	MQX_TICK_STRUCT ticks;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	/* Initialize output list to NULL */
	*list = NULL;

	msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return 0;
	}

	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = OVRDUE_LIST;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return 0;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// Send message
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	_msgq_send(msg_req_ptr);
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	/* Grab info and free message */
	ovrdue_list = (QUEUE_STRUCT_PTR)msg_res_ptr->TASK_LIST;
	status = msg_res_ptr->STATUS;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return 0;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	if(status != SUCCESSFUL)
	{
		return 0;
	}

	// copy pointer to active list
	ovrdue_list_itr = (SCH_TASK_NODE_PTR)_queue_head(ovrdue_list);

	/* Handle first element */
	if(ovrdue_list_itr != NULL)
	{
		/* Allocate new list element */
		struct task_list* list_item;
		list_item = (struct task_list *)_partition_alloc(task_list_partition);
		if(_task_get_error() != MQX_OK){
			printf("Failed to allocate task memory from partition.\n");
			printf("Error code: %x\n", (unsigned int)_task_get_error());
			_task_set_error(MQX_OK);
			return FALSE;
		}

		/* Populate list element */
		list_item->tid	= ovrdue_list_itr->TID;
		list_item->creation_time = ovrdue_list_itr->CREATION_TIME;
		list_item->deadline = ovrdue_list_itr->ABS_DEADLINE
				- ovrdue_list_itr->CREATION_TIME;
		list_item->task_type = ovrdue_list_itr->TASK_TYPE;
		list_item->next_cell = NULL;
		list_item->previous_cell = NULL;

		/* Add list item to user list */
		list_itr = list_item;

		/* Go to next element */
		ovrdue_list_itr = (SCH_TASK_NODE_PTR)_queue_next(ovrdue_list, ovrdue_list_itr);
	}

	*list = list_itr;

	/* Handle all subsequent items */
	while (ovrdue_list_itr != NULL)
	{
		/* Allocate new list element */
		struct task_list* list_item;
		list_item = (struct task_list *)_partition_alloc(task_list_partition);
		if(_task_get_error() != MQX_OK){
			printf("Failed to allocate task memory from partition.\n");
			printf("Error code: %x\n", (unsigned int)_task_get_error());
			_task_set_error(MQX_OK);
			return FALSE;
		}

		/* Populate list element */
		list_item->tid	= ovrdue_list_itr->TID;
		list_item->creation_time = ovrdue_list_itr->CREATION_TIME;
		list_item->deadline = ovrdue_list_itr->ABS_DEADLINE
				- ovrdue_list_itr->CREATION_TIME;
		list_item->task_type = ovrdue_list_itr->TASK_TYPE;
		list_item->next_cell = NULL;
		list_item->previous_cell = list_itr;

		/* Add item to end of list */
		list_itr->next_cell = list_item;

		/* Move to next element */
		list_itr = list_item;

		/* Go to next element */
		ovrdue_list_itr = (SCH_TASK_NODE_PTR)_queue_next(ovrdue_list, ovrdue_list_itr);
	}

	// LOCK MUTEX
	error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return TRUE;
}

uint32_t dd_return_overhead(time_t * ovrhd){

	_queue_id msg_qid;
	_status_sch status;
	time_t time;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	MQX_TICK_STRUCT ticks;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	/*msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return FALSE;
	}


	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = OVERHEAD;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	// Send message
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	_msgq_send(msg_req_ptr);
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	// Grab info and free message
	status = msg_res_ptr->STATUS;
	time = msg_res_ptr->TIMER;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	*overhead = time;

	if(status != SUCCESSFUL){
		printf("Failure to get Overhead! \n");
		return FALSE;
	} */



	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	*ovrhd = overhead / _time_get_hwticks_per_tick();

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return TRUE;

}

uint32_t dd_reset_scheduler()
{
	_queue_id msg_qid;
	_status_sch status;
	time_t time;
	SCHEDULER_REQUEST_MSG_PTR msg_req_ptr;
	SCHEDULER_RESPONSE_MSG_PTR msg_res_ptr;
	MQX_TICK_STRUCT ticks;
	time_t start, end;

	_time_get_elapsed_ticks(&ticks);
	start = ticks.HW_TICKS;

	msg_req_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(req_msg_pool);
	if(msg_req_ptr == NULL){
		printf("Could not allocate a message from the Scheduler\n");
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Setup the message
	msg_req_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SCHEDULER_QUEUE);
	msg_req_ptr->CMD_ID = RESET;

	// LOCK MUTEX
	_mqx_uint error = _mutex_lock(&scheduler_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	// Message queue initialization code
	msg_qid = _msgq_open(DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open Schedule message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	// Send message
	msg_req_ptr->HEADER.SOURCE_QID = msg_qid;
	_msgq_send(msg_req_ptr);
	if(_task_get_error() != MQX_OK){
		printf("Failed to send message from ______ \n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	 // Wait for the return message:
	msg_res_ptr = _msgq_receive(msg_qid, 0);

	/* Grab info and free message */
	status = msg_res_ptr->STATUS;
	_msg_free(msg_res_ptr);

	_msgq_close(msg_qid);
	if(_task_get_error() != MQX_OK){
		printf("Failed to close queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_mutex_unlock(&scheduler_mutex);
		return FALSE;
	}

	// UNLOCK MUTEX
	_mutex_unlock(&scheduler_mutex);

	if(status != SUCCESSFUL){
		printf("Failed to reset scheduler! \n");
		return FALSE;
	}

	// LOCK MUTEX
	error = _mutex_lock(&overhead_mutex);
	if (error != MQX_OK) {
		printf("Mutex lock failed.\n");
		printf("Error: %x\n", error);
		_task_set_error(MQX_OK);
		return FALSE;
	}

	_time_get_elapsed_ticks(&ticks);
	end = ticks.HW_TICKS;
	overhead += end - start;

	// UNLOCK MUTEX
	_mutex_unlock(&overhead_mutex);

	return TRUE;
}

