/* ###################################################################
**     Filename    : os_tasks.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-06, 17:17, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         handler_task - void handler_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         

#ifndef __os_tasks_H
#define __os_tasks_H
/* MODULE os_tasks */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "mqx_ksdk.h"
#include "uart1.h"
#include "fsl_mpu1.h"
#include "fsl_hwtimer1.h"
#include "MainTask.h"
#include "handlerTask.h"
#include "myUART.h"

#include <mqx.h>
#include <message.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define DATA_SIZE	  8
#define RX_QUEUE_SIZE 8
#define USER_QUEUE_SIZE 32

/* Queue IDs */
#define RX_QUEUE      8
#define USER_QUEUE    9

typedef enum request_cmd
{
	READ_PRIV = 0,
	READ_PRIV_ACK,
	WRITE_PRIV,
	WRITE_PRIV_ACK,
	CLOSE,
	CLOSE_ACK,
	READ,
	READ_ACK,
	WRITE,
	WRITE_ACK,
	UNKNOWN_ACK
} _request_cmd;

typedef enum status
{
	FAILURE = 0,
	SUCCESS
} _status;

typedef struct rx_message
{
	MESSAGE_HEADER_STRUCT   HEADER;
	uint8_t					DATA;
	uint8_t					RESERVED[3];
} RX_MESSAGE, * RX_MESSAGE_PTR;

typedef struct user_message
{
	MESSAGE_HEADER_STRUCT   HEADER;
	_request_cmd			CMD_ID;
	_status					STATUS;
	_task_id				TASK_ID;
	uint8_t					DATA[DATA_SIZE];
} USER_MESSAGE, * USER_MESSAGE_PTR;

extern _pool_id rx_msg_pool;
extern _pool_id user_msg_pool;

/*
** ===================================================================
**     Callback    : handler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void handler_task(os_task_param_t task_init_data);
/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __os_tasks_H*/
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
