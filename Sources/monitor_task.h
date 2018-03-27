/* ###################################################################
**     Filename    : monitor_task.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-24, 16:50, # CodeGen: 12
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         monitor_task - void monitor_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file monitor_task.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup monitor_task_module monitor_task module documentation
**  @{
*/         

#ifndef __monitor_task_H
#define __monitor_task_H
/* MODULE monitor_task */

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
#include "gpio1.h"
#include "SW3Task.h"
#include "SW2Task.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data);


/*
** ===================================================================
**     Callback    : idle_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void idle_task(os_task_param_t task_init_data);

/* END monitor_task */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __monitor_task_H*/
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
