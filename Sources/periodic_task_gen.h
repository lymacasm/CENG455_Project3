/* ###################################################################
**     Filename    : periodic_task_gen.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-24, 15:28, # CodeGen: 10
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         periodic_task_gen - void periodic_task_gen(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file periodic_task_gen.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup periodic_task_gen_module periodic_task_gen module documentation
**  @{
*/         

#ifndef __periodic_task_gen_H
#define __periodic_task_gen_H
/* MODULE periodic_task_gen */

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

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct periodic_task
{
	time_t period;
	time_t exec_time;
	time_t deadline;
	time_t phase;
	time_t executions;
} PERIODIC_TASK, PERIODIC_TASK_PTR;

/*
** ===================================================================
**     Callback    : periodic_task_gen
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void periodic_task_gen(os_task_param_t task_init_data);


/*
** ===================================================================
**     Callback    : periodic_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void periodic_task(os_task_param_t task_init_data);

/* END periodic_task_gen */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __periodic_task_gen_H*/
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
