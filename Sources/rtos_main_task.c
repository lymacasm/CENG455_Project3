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
#include "uart_handler.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define MAIN_QUEUE 12

/* User includes (#include below this line is not maintained by Processor Expert) */

/* Initialization of Processor Expert components function prototype */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
extern void PEX_components_init(void);
#endif 

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
	_queue_id main_qid;
	_queue_id write_qid;

	/* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
	PEX_components_init();
#endif 

	/* End of Processor Expert components initialization.  */
	main_qid = _msgq_open(MAIN_QUEUE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open main message pool.\n");
		printf("Error code: %x\n", _task_get_error());
		_task_block();
	}

	printf("Gonna get read/write privileges.\n");
	if(OpenR(main_qid)) printf("Successfully got read privileges.\n");
	write_qid = OpenW();
	if(write_qid) printf("Successfully got write privileges.\n");

#ifdef PEX_USE_RTOS
	while (1) {
#endif
		/* Write your code here ... */
		char string[DATA_SIZE + 1];
		if(!_get_line(string))
		{
			printf("main task: failed to get line...\n");
			_task_block();
		}
		if(strcmp("Hello", string) == 0)
		{
			_putline(write_qid, "Hi!\r");
		}
		else if(strcmp("CLOSE", string) == 0)
		{
			printf("Closing connections.\n");
			if(Close()) printf("Closed connections!\n");
			else printf("Failed to close connections!\n");
			printf("Sending a read request\n");
			if(_get_line(string)) printf("I got a line even when I didn't have privileges...\n");
			printf("Sending a write request\n");
			if(_putline(write_qid, "Hola\r")) printf("I printed without write privileges...\n");
			printf("OpenR\n");
			if(OpenR(main_qid)) printf("Successfully got read privileges.\n");
			printf("OpenW\n");
			write_qid = OpenW();
			if(write_qid) printf("Successfully got write privileges.\n");
		}

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
