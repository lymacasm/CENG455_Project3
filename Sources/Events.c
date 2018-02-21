/* ###################################################################
**     Filename    : Events.c
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-06, 17:09, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

/* queue id */
#define RX_QUEUE_SENDING 10

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : myUART_RxCallback
**     Description : This callback occurs when data are received.
**     Parameters  :
**       instance - The UART instance number.
**       uartState - A pointer to the UART driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void myUART_RxCallback(uint32_t instance, void * uartState)
{
	static uint8_t message_pool_closed = 1;
	static _queue_id rx_qid;
	RX_MESSAGE_PTR msg_ptr;

	/* Message queue initialization code */
	if(message_pool_closed)
	{
		rx_qid = _msgq_open((_queue_number)RX_QUEUE_SENDING, 0);
		if(_task_get_error() != MQX_OK)
		{
			printf("Failed to open RX ISR sending message queue.\n");
			printf("Error code: %x\n", MQX_OK);
			return;
		}
		message_pool_closed = 0; // Set to false for not closed
	}

  /* Write your code here ... */
	msg_ptr = (RX_MESSAGE_PTR)_msg_alloc(rx_msg_pool);
	if(msg_ptr == NULL)
	{
		printf("Could not allocate a message from the RX ISR\n");
		return;
	}

	/* Setup the message */
	msg_ptr->HEADER.SOURCE_QID = rx_qid;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, RX_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(RX_MESSAGE);
	msg_ptr->DATA = myRxBuff[0];

	/* Check if msgq_get_id worked */
	/* NOTE: No MQX call can be inserted between _msgq_get_id and this check */
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to get the queue ID for the RX queue.\n");
		printf("Error code: %x\n", MQX_OK);
		return;
	}

	/* Send message */
	_msgq_send(msg_ptr);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to send message from RX ISR to Handler.\n");
		printf("Error code: %x\n", MQX_OK);
		return;
	}
	//UART_DRV_SendData(myUART_IDX, myRxBuff, sizeof(myRxBuff));
}

/* END Events */

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
