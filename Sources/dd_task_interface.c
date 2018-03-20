/*
 * edf_task_interface.c
 *
 *  Created on: Mar 13, 2018
 *      Author: lymacasm
 */

#define DD_INTERFACE_QUEUE 9

_task_id dd_tcreate(uint32_t template_index, time_t deadline){
	/*
	 This primitive, creates a deadline driven scheduled task. It follows the steps outlined below
		1. Opens a queue
		2. Creates the task specified and assigns it the minimum priority possible
		3. Composes a create_task_message and sends it to the DD-scheduler
		4. Waits for a reply at the queue it created above
		5. Once the reply is received, it obtains it
		6. Destroys the queue
		7. Returns to the invoking task
	template_index is the template index of the task to be created.
	deadline is the number of clock ticks to the task's deadline.
	It returns the task_id of the created task, or an error. The error is either an MQX task creation
	error or a DD-scheduler specific error (to be determined).
	 */
	_queue_id user_qid;
	SCHEDULER_REQUEST_MSG_PTR msg_ptr;


	// Message queue initialization code
	user_qid = _msgq_open((_queue_number)DD_INTERFACE_QUEUE, 0);
	if(_task_get_error() != MQX_OK){
		printf("Failed to open USER sending message queue.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_set_error(MQX_OK);
		_msgq_close(user_qid);
		return FALSE;
	}

	msg_ptr = (SCHEDULER_REQUEST_MSG_PTR)_msg_alloc(user_msg_pool);
	if(msg_ptr == NULL){
		printf("Could not allocate a message from the USER\n");
		_task_set_error(MQX_OK);
		_msgq_close(user_qid);
		_mutex_unlock(&print_mutex);
		return FALSE;
	}
}

_task_id dd_delete(_task_id task_id){
	/*
	When the function dd_delete is called, the specified task is deleted.
	This primitive deletes the task specified. It parallels the structure of the dd_tcreate as outlined
	above.
	 */
	_queue_id user_qid;
	SCHEDULER_REQUEST_MSG_PTR msg_ptr;
}

uint32_t dd_return_active_list(struct task_list ** list){
	/*
	 This primitive requests the DD-scheduler for the list of active tasks and returns this information
	to the requestor.
	The designer must decide whether the list should be copied and sent to the requestor, or simply a
	pointer pointing to the start of the list could suffice. Analyze the alternatives and justify your implementation
	choice.
	 */

}

uint32_t dd_return_overdue_list(struct overdue_tasks ** list){
	/*
	This primitive requests the DD-scheduler for the list of overdue tasks and returns this information
	to the requestor. Similar in structure as the dd_return_active_list primitive.
	 */

}


