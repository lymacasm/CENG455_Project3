/* ###################################################################
**     Filename    : scheduler.h
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
** @file scheduler.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup scheduler_module scheduler module documentation
**  @{
*/         

#ifndef __scheduler_H
#define __scheduler_H
/* MODULE scheduler */

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
#include "UserTask.h"
#include "SchedulerTask.h"
#include "PeriodicTaskGen.h"
#include "PeriodicTask.h"
#include "MonitorTask.h"
#include "IdleTask.h"

#include "dd_task_interface.h"
#include <message.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define SCHEDULER_QUEUE 8

/* Node used to store tasks in list */
typedef struct sch_task_node
{
	QUEUE_ELEMENT_STRUCT 	HEADER;
	_task_id				TID;
	time_t					ABS_DEADLINE;
	time_t					CREATION_TIME;
	uint32_t				TASK_TYPE;
	_mqx_uint 				TASK_PRIORITY;
} SCH_TASK_NODE, * SCH_TASK_NODE_PTR;

/* Enumeration of different command ID's */
typedef enum scheduler_cmd
{
	CREATE = 0,
	CREATE_ACK,
	DELETE,
	DELETE_ACK,
	ACTIVE_LIST,
	ACTIVE_LIST_ACK,
	OVRDUE_LIST,
	OVRDUE_LIST_ACK,
	OVERHEAD,
	OVERHEAD_ACK,
	RESET,
	RESET_ACK,
	FAILURE_ACK,
	SCH_UNKNOWN_ACK
} _scheduler_cmd;

typedef enum status_sch
{
	FAILED = 0,
	SUCCESSFUL,
	OVERDUE
} _status_sch;

/* The request message struct that is received by scheduler */
typedef struct scheduler_request_msg
{
	MESSAGE_HEADER_STRUCT   HEADER;
	_scheduler_cmd			CMD_ID;
	struct task_list*		TASK_INFO;
	uint32_t				PARAMETER;
} SCHEDULER_REQUEST_MSG, * SCHEDULER_REQUEST_MSG_PTR;

/* The response message struct that is sent by scheduler */
typedef struct scheduler_response_msg
{
	MESSAGE_HEADER_STRUCT   HEADER;
	_scheduler_cmd			ACK_ID;
	_task_id				TID;
	QUEUE_STRUCT_PTR		TASK_LIST;
	_status_sch				STATUS;
	time_t					TIMER;
} SCHEDULER_RESPONSE_MSG, * SCHEDULER_RESPONSE_MSG_PTR;

/*
** ===================================================================
**     Callback    : scheduler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void scheduler_task(os_task_param_t task_init_data);

extern _pool_id req_msg_pool;

/* END scheduler */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __scheduler_H*/
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
