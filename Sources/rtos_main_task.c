/* ###################################################################
**     Filename    : rtos_main_task.c
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
**         main_task - void main_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file rtos_main_task.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup rtos_main_task_module rtos_main_task module documentation
**  @{
*/         
/* MODULE rtos_main_task */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "user_task.h"
#include "scheduler.h"
#include "periodic_task_gen.h"
#include "monitor_task.h"
#include "uart_handler.h"
#include "dd_task_interface.h"
#include <stdio.h>
#include <mutex.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define NUM_OF_TASK 15
#define MAIN_QUEUE 5


/* User includes (#include below this line is not maintained by Processor Expert) */

/* Initialization of Processor Expert components function prototype */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
extern void PEX_components_init(void);
#endif 

extern _queue_id write_qid;

/*
** ===================================================================
**     Callback    : main_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void main_task(os_task_param_t task_init_data)
{
	/* Write your local variable definition here */
	_queue_id 			main_qid;
	_queue_id			my_write_qid;
	_mqx_uint			min_priority;
	uint32_t  			i;
	MUTEX_ATTR_STRUCT 	mutexattr;

	dd_init();

	/* Initialize mutex attributes: */
	if (_mutatr_init(&mutexattr) != MQX_OK) {
		printf("Initializing mutex attributes failed.\n");
		_mqx_exit(0);
	}

	/* Initialize the mutex: */
	if (_mutex_init(&print_mutex, &mutexattr) != MQX_OK) {
		printf("Initializing print mutex failed.\n");
		 _mqx_exit(0);
	}

	/* Initialize mutex attributes: */
	if (_mutatr_init(&mutexattr) != MQX_OK) {
		printf("Initializing mutex attributes failed.\n");
		_mqx_exit(0);
	}

	/* Initialize the mutex: */
	if (_mutex_init(&scheduler_mutex, &mutexattr) != MQX_OK) {
		printf("Initializing scheduler mutex failed.\n");
		 _mqx_exit(0);
	}

	/* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
	PEX_components_init();
#endif 

	_task_block();

	/* End of Processor Expert components initialization.  */
	main_qid = _msgq_open(MAIN_QUEUE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open main message pool.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_block();
	}

	/* Create User Task */
	 for (i = 0; i < NUM_OF_TASK; i++) {
	 _task_create(0, USERTASK_TASK, i);
	 }

#ifdef PEX_USE_RTOS
	while (1) {
#endif
		char string[DATA_SIZE + 1];


		/* Make sure I can open read privilege */
		if (!OpenR(main_qid)){
			printf("Master: Failed to get read privileges\n");
			break;
		}

		/* Make sure I can't get read privilege again */
		if(OpenR(main_qid))
		{
			printf("Master: Was able to acquire read privileges a second time.\n");
			break;
		}

		/* Try and get write privilege */
		my_write_qid = OpenW();
		if (my_write_qid != 0)
		{
			printf("Master: Successfully got write privileges\n");
			write_qid = my_write_qid;

			/* Make sure I can't get write privilege a second time */
			if(OpenW() != 0)
			{
				printf("Master: Got write privilege a second time.\n");
				break;
			}
		}

		/* Try and get a line */
		if(_get_line(string))
		{
			printf("Master: %s\n", string);
			char write_string[DATA_SIZE + 16];
			sprintf(write_string, "Master: %s\n\r", string);

			/* Try and write a line */
			if(my_write_qid != 0)
			{
				if(!_putline(my_write_qid, write_string))
				{
					printf("Master: Couldn't write when I should have been able to.\n");
					break;
				}
			}

			/* Try and write even if I don't have write privilege */
			else
			{
				if(_putline(write_qid, write_string))
				{
					printf("Master: Wrote when I shouldn't have.\n");
					break;
				}
			}

		}
		else
		{
			printf("Master: Failed to get line\n");
			break;
		}

		/* Try and remove privileges */
		if(!Close())
		{
			printf("Master: Failed to close privileges\n");
			break;
		}

		/* Try to remove privileges again */
		if(Close())
		{
			printf("Master: Was able to close privileges right after closing.\n");
			break;
		}

		/* Try and read */
		if(_get_line(string))
		{
			printf("Master: Could read when I shouldn't have.\n");
			break;
		}

		my_write_qid = 0;

		OSA_TimeDelay(10);    /* Example code (for task release) */
   
#ifdef PEX_USE_RTOS   
	}
#endif    
	_task_block();
}

/* END rtos_main_task */

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
