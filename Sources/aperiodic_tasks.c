/* ###################################################################
**     Filename    : aperiodic_tasks.c
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-26, 18:35, # CodeGen: 16
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         switch2_task - void switch2_task(os_task_param_t task_init_data);
**         switch3_task - void switch3_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file aperiodic_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup aperiodic_tasks_module aperiodic_tasks module documentation
**  @{
*/         
/* MODULE aperiodic_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "user_task.h"
#include "scheduler.h"
#include "periodic_task_gen.h"
#include "monitor_task.h"
#include "aperiodic_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : switch2_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void switch2_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  
#ifdef PEX_USE_RTOS

	// turn led on/off
	printf("APeriodic Task created! \n");

	dd_delete(_task_get_id());
	_task_block();

  while (1) {
#endif
    /* Write your code here ... */
    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : switch3_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void switch3_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END aperiodic_tasks */

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
