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
#include "aperiodic_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define IDLE_TICK_DELAY 1
#define MONITOR_TICK_DELAY 3000

volatile uint32_t idle_counter = 0;

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
	uint32_t check_overhead;
	uint32_t overhead_ticks;
	uint32_t overhead_micro_ticks;
	uint32_t wait_time;
	uint32_t pros_utilization;
	uint32_t overhead;
	uint32_t idle_last = 0, idle_delta;
	uint32_t overhead_ticks_last = 0;;
	uint32_t overhead_micro_ticks_last = 0;
	uint32_t total_last = 0;
	uint32_t counter = 0;
	struct task_list * list;
	MQX_TICK_STRUCT current_t;
	time_t total_time = 0;
	time_t schdeuler_overhead = 0;		// to be calculated
	_mqx_uint min_priority;
	_mqx_uint old_priority;

	/* Set yourself to lowest priority */
	/*min_priority = _sched_get_min_priority(0);
	if(_task_set_priority(_task_get_id(), 2, &old_priority) != MQX_OK)
	{
		printf("Failed to set the priority of monitor to lowest (%u)\n", (unsigned int)min_priority);
		_task_block();
	}*/

	_task_get_priority(_task_get_id(), &min_priority);
	printf("Monitor priority is %u", min_priority);
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif

	check_overdue = dd_return_overdue_list(&list);
	if ((check_overdue != 0) && (list != NULL)){
		printf("\nResetting scheduler\n");
		dd_reset_scheduler();
		continue;
	}


	// System overhead
	check_overhead = dd_return_overhead(&schdeuler_overhead);
	if (check_overhead == FALSE){
		printf("Failed to calculate overhead! \n");
		continue;
	}

	overhead_ticks = schdeuler_overhead / _time_get_hwticks_per_tick();
	overhead_micro_ticks = schdeuler_overhead * 1000000 / _time_get_hwticks_per_tick();

	// Absolute time
	_time_get_elapsed_ticks(&current_t);
	total_time = current_t.TICKS[0];
	idle_delta = idle_counter - idle_last;
	idle_last = idle_counter;
	// Calculating Processor Utilization
	wait_time = (100*((idle_delta*IDLE_TICK_DELAY)+
			(overhead_ticks - overhead_ticks_last)))/(total_time - total_last);
	pros_utilization = 100 - wait_time;
	overhead = (overhead_micro_ticks - overhead_micro_ticks_last) / (total_time - total_last);


	printf("\nIdle:%u, Sch:%u, Total:%u\n", idle_delta*IDLE_TICK_DELAY,
			(overhead_ticks - overhead_ticks_last), (total_time - total_last));
	printf("Scheduler Overhead = %ux10^-6%%\n", (unsigned int)overhead);
	printf("Processor Utilization = %u%%\n", (unsigned int)pros_utilization);


	idle_last = idle_counter;
	overhead_ticks_last = overhead_ticks;
	overhead_micro_ticks_last = overhead_micro_ticks;
	total_last = total_time;

	_time_delay_ticks(MONITOR_TICK_DELAY);

	counter++;
    
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
	_mqx_uint min_priority;
	_mqx_uint old_priority;

	/* Set yourself to second lowest priority */
	min_priority = _sched_get_min_priority(0);
	if(_task_set_priority(_task_get_id(), min_priority, &old_priority) != MQX_OK)
	{
		printf("Failed to set the priority of monitor to lowest (%u)\n", (unsigned int)min_priority);
		_task_block();
	}
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
    
    
    _time_delay_ticks(IDLE_TICK_DELAY);
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
