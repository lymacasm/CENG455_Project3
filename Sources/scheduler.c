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

#define REQ_POOL_SIZE 8
#define RSP_POOL_SIZE REQ_POOL_SIZE

_pool_id req_msg_pool;
_pool_id rsp_msg_pool;

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
		printf("Error code: %x\n", MQX_OK);
		_task_set_error(MQX_OK);
		_task_block();
	}

	/* Create request message pool */
	rsp_msg_pool = _msgpool_create(sizeof(SCHEDULER_RESPONSE_MSG), RSP_POOL_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open response message pool.\n");
		printf("Error code: %x\n", MQX_OK);
		_task_set_error(MQX_OK);
		_task_block();
	}

#ifdef PEX_USE_RTOS
	while (1) {
#endif
		/* Write your code here ... */
		time_t timeout = 0;
		if(!_queue_is_empty(&active_list))
		{
			/* Get the soonest deadline */
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
			case(CREATE):
				response_msg->ACK_ID = CREATE_ACK;
				break;
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
