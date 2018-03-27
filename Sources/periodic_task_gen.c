/* ###################################################################
**     Filename    : periodic_task_gen.c
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
** @file periodic_task_gen.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup periodic_task_gen_module periodic_task_gen module documentation
**  @{
*/         
/* MODULE periodic_task_gen */

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

#define TASK_LIST_SIZE 5

PERIODIC_TASK task_list[TASK_LIST_SIZE] = {
		/*
	   Period, Ex.Time, Deadline, Phase, Execution Cycles
		 */
		//{1000, 200,		600,	  0, 		0},
		//{500,  100,		350, 	  0, 		0},
		//{100,  20,		70, 	  0, 		0},
		//{5000, 100, 	150, 	  0, 		0},
		{500, 60, 	500, 	  0, 		0}
};

/*
#define TASK_LIST_SIZE 1

PERIODIC_TASK task_list[TASK_LIST_SIZE] = {
	   {5000,	1000,		4000,	  1000, 		0}
};*/

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : periodic_task_gen
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void periodic_task_gen(os_task_param_t task_init_data)
{
	/* Write your local variable definition here */
	MQX_TICK_STRUCT current_time;
  
#ifdef PEX_USE_RTOS
	while (1) {
#endif
		_time_get_elapsed_ticks(&current_time);

		/* Find the next task that needs to be created */
		uint32_t i;
		uint32_t min_task_idx = 0;
		time_t min_task_time = 0x7FFFFFFF;
		/* Find the min wait time to the next periodic task creation */
		for(i = 0; i < TASK_LIST_SIZE; i++)
		{
			/* Calculate the wait time for current periodic task */
			time_t time_to_wait = (current_time.TICKS[0] % task_list[i].period)
					+ task_list[i].phase;

			/* Update min wait time information */
			if( time_to_wait < min_task_time )
			{
				min_task_time = time_to_wait;
				min_task_idx = i;
			}
		}
   
		/* Delay for min wait time */
		_time_delay_ticks(min_task_time);
    
		/* Create the periodic task */
		dd_tcreate(PERIODICTASK_TASK, min_task_idx, task_list[min_task_idx].deadline);

		/* Create monitor task */
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : periodic_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void periodic_task(os_task_param_t task_init_data)
{
	MQX_TICK_STRUCT start_time;

	/* Get the current time */
	_time_get_elapsed_ticks(&start_time);

	/* Delay for execution time */
	while(1)
	{
		MQX_TICK_STRUCT now_time;
		_time_get_elapsed_ticks(&now_time);
		if((now_time.TICKS[0] - start_time.TICKS[0]) > task_list[task_init_data].exec_time)
		{
			printf("Deadline met!\n");

			/* Delete and deschedule myself */
			dd_delete(_task_get_id());

			/* Shouldn't reach here */
			_task_block();
		}
	}
}

/* END periodic_task_gen */

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
