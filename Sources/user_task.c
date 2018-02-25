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
#include "uart_handler.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define USER_QUEUE_BASE 6

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
	_queue_id write_qid = 0;

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


	printf("User%d: OpenR\n", task_init_data);
	if (OpenR(user_qid)){
		printf("User%d: Successfully got read privileges\n", task_init_data);
	}
	printf("User%d: OpenW\n", task_init_data);
	write_qid = OpenW();
	if (write_qid != 0)
	{
		printf("User%d: Successfully got write privileges\n", task_init_data);
	}

	if(_get_line(string) && write_qid != 0)
	{
		char write_string[DATA_SIZE + 1];
		sprintf(write_string, "User%d: %s\n\r", task_init_data, string);
		_putline(write_qid, write_string);
	}
	else
	{
		printf("User%d: Failed to get line\n", task_init_data);
	}
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    break;
    
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
