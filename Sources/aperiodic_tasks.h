/* ###################################################################
**     Filename    : aperiodic_tasks.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-26, 19:57, # CodeGen: 20
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         switch3_task - void switch3_task(os_task_param_t task_init_data);
**         switch2_task - void switch2_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file aperiodic_tasks.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup aperiodic_tasks_module aperiodic_tasks module documentation
**  @{
*/         

#ifndef __aperiodic_tasks_H
#define __aperiodic_tasks_H
/* MODULE aperiodic_tasks */

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
#include "Switch2Task.h"
#include "Switch3Task.h"
#include "AperiodicTaskGen.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Callback    : switch3_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void switch3_task(os_task_param_t task_init_data);

/*
** ===================================================================
**     Callback    : switch2_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void switch2_task(os_task_param_t task_init_data);


/*
** ===================================================================
**     Callback    : aperiodic_task_gen
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void aperiodic_task_gen(os_task_param_t task_init_data);

/* END aperiodic_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __aperiodic_tasks_H*/
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
