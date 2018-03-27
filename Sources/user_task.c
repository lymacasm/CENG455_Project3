/* ###################################################################
**     Filename    : user_task.c
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-24, 19:11, # CodeGen: 3
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         user_task - void user_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file user_task.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup user_task_module user_task module documentation
**  @{
*/         
/* MODULE user_task */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "user_task.h"
#include "scheduler.h"
#include "periodic_task_gen.h"
#include "monitor_task.h"
#include "uart_handler.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define USER_QUEUE_BASE 10

_queue_id write_qid = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{
	_queue_id user_qid;
	_queue_id my_write_qid = 0;

	printf("User task %d created\n", task_init_data);

	user_qid = _msgq_open((_queue_number)(USER_QUEUE_BASE +
			task_init_data), 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("User%d: Failed to open user message queue!\n", task_init_data);
		printf("User%d: Error %x", _task_get_error());
		_task_set_error(MQX_OK);
		_task_block();
	}


#ifdef PEX_USE_RTOS
  while (1) {
#endif
    char string[DATA_SIZE + 1];


    /* Make sure I can open read privilege */
	if (!OpenR(user_qid)){
		printf("User%d: Failed to get read privileges\n", task_init_data);
		break;
	}

	/* Make sure I can't get read privilege again */
	if(OpenR(user_qid))
	{
		printf("User%d: Was able to acquire read privileges a second time.\n", task_init_data);
		break;
	}

	/* Try and get write privilege */
	my_write_qid = OpenW();
	if (my_write_qid != 0)
	{
		printf("User%d: Successfully got write privileges\n", task_init_data);
		write_qid = my_write_qid;

		/* Make sure I can't get write privilege a second time */
		if(OpenW() != 0)
		{
			printf("User%d: Got write privilege a second time.\n", task_init_data);
			break;
		}
	}

	/* Try and get a line */
	if(_get_line(string))
	{
		printf("User%d: %s\n", task_init_data, string);
		char write_string[DATA_SIZE + 16];
		sprintf(write_string, "User%d: %s\n\r", task_init_data, string);

		/* Try and write a line */
		if(my_write_qid != 0)
		{
			if(!_putline(my_write_qid, write_string))
			{
				printf("User%d: Couldn't write when I should have been able to.\n", task_init_data);
				break;
			}
		}

		/* Try and write even if I don't have write privilege */
		else
		{
			if(_putline(write_qid, write_string))
			{
				printf("User%d: Wrote when I shouldn't have.\n", task_init_data);
				break;
			}
		}

	}
	else
	{
		printf("User%d: Failed to get line\n", task_init_data);
		break;
	}

	/* Try and remove privileges */
	if(!Close())
	{
		printf("User%d: Failed to close privileges\n", task_init_data);
		break;
	}

	/* Try to remove privileges again */
	if(Close())
	{
		printf("User%d: Was able to close privileges right after closing.\n", task_init_data);
		break;
	}

	/* Try and read */
	if(_get_line(string))
	{
		printf("User%d: Could read when I shouldn't have.\n", task_init_data);
		break;
	}

	my_write_qid = 0;

    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
#ifdef PEX_USE_RTOS   
  }
#endif
  	  _task_block();
}

/* END user_task */

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
