/* ###################################################################
**     Filename    : monitor_task.c
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
** @file monitor_task.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup monitor_task_module monitor_task module documentation
**  @{
*/         
/* MODULE monitor_task */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "user_task.h"
#include "scheduler.h"
#include "periodic_task_gen.h"
#include "monitor_task.h"

#ifdef __cplusplus
extern "C" {
#endif 

uint32_t idle_counter = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : monitor_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void monitor_task(os_task_param_t task_init_data)
{
	uint32_t check_overdue;
	uint32_t efficiency;
	uint32_t pros_utilization;
	struct task_list * list;
	MQX_TICK_STRUCT_PTR current_t;
	time_t total_time = 0;
	time_t schdeuler_overhead;		// to be calculated

  
#ifdef PEX_USE_RTOS
  while (1) {
#endif

	// check overdue list every 5 iteration
	if(counter % 5 == 0){
		check_overdue = dd_return_overdue_list(*list);
		if (check_overdue != 0 && *list != NULL){
			printf("Scheduler not performing at Optimal level \n");
			return;
		}
	}

	// Absolute time
	_time_get_ticks(current_t);
	total_time = current_t->TICKS[0];		// could use HW_TICKS ?

	// Calculating Processor Utilization
	efficiency = ((idle_counter*10)+(schdeuler_overhead))/total_time;
	printf("Efficiency = %lX \n",efficiency);  // for debugging purpose
	pros_utilization = 1 - efficiency;
    printf("Processor Utilization = %lX \n",pros_utilization);
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : idle_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void idle_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
    idle_counter++;
   
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END monitor_task */

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
