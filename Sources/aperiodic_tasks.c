/* ###################################################################
**     Filename    : aperiodic_tasks.c
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
#include <event.h>

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

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
	printf("Swtich3 Pressed!\n");

	/* Write your code here ... */
	GPIO_DRV_SetPinOutput(LEDRGB_GREEN);
	GPIO_DRV_SetPinOutput(LEDRGB_BLUE);
	GPIO_DRV_SetPinOutput(LEDRGB_RED);

	OSA_TimeDelay(500);                 /* Example code (for task release) */

	GPIO_DRV_ClearPinOutput(LEDRGB_GREEN);
	GPIO_DRV_ClearPinOutput(LEDRGB_BLUE);
	GPIO_DRV_ClearPinOutput(LEDRGB_RED);

	OSA_TimeDelay(1000);
	int i;
	for(i = 0; i < 3; i++)
	{
		GPIO_DRV_ClearPinOutput(LEDRGB_RED);
		GPIO_DRV_SetPinOutput(LEDRGB_GREEN);

		OSA_TimeDelay(200);

		GPIO_DRV_ClearPinOutput(LEDRGB_GREEN);
		GPIO_DRV_SetPinOutput(LEDRGB_BLUE);

		OSA_TimeDelay(200);

		GPIO_DRV_ClearPinOutput(LEDRGB_BLUE);
		GPIO_DRV_SetPinOutput(LEDRGB_RED);

		OSA_TimeDelay(200);
	}


	dd_delete(_task_get_id());
	_task_block();
}

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
	printf("Swtich2 Pressed!\n");
	int i;
#ifdef PEX_USE_RTOS
	for (i = 0; i < 10; i++) {
#endif
		/* Write your code here ... */
		GPIO_DRV_TogglePinOutput(LEDRGB_BLUE);

		OSA_TimeDelay(50);                 /* Example code (for task release) */

		GPIO_DRV_TogglePinOutput(LEDRGB_RED);

		OSA_TimeDelay(50);                 /* Example code (for task release) */
    
#ifdef PEX_USE_RTOS   
	}
#endif    

	dd_delete(_task_get_id());
	_task_block();
}

/*
** ===================================================================
**     Callback    : aperiodic_task_gen
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void aperiodic_task_gen(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
	void * event_ptr;

	if(_event_create("aperiodic") != MQX_OK)
	{
		printf("Failed to make event!\n");
		_task_block();
	}

	if(_event_open("aperiodic", &event_ptr) != MQX_OK)
	{
		printf("Failed to open event!\n");
		_task_block();
	}
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
	uint32_t event_bits;
    
    if(_event_wait_any(event_ptr, 0x3, 0) != MQX_OK)
    {
    	printf("Event wait failed\n");
    	_task_block();
    }
    
    if(_event_get_value(event_ptr, &event_bits) == MQX_OK)
    {
    	if(event_bits & 0x1)
    	{
    		dd_tcreate(SWITCH2TASK_TASK, 0, 500);
    	}
    	if(event_bits & 0x2)
    	{
    		dd_tcreate(SWITCH3TASK_TASK, 0, 1000);
    	}
    }
    
    if(_event_clear(event_ptr, 0x3) != MQX_OK)
    {
    	printf("Event Clear Failed\n");
    	_task_block();
    }
    
    
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
