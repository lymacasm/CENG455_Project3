/** ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : mqx_sdk_config.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-06, 17:17, # CodeGen: 1
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file mqx_sdk_config.h                                                  
** @version 01.00
*/         
/*!
**  @addtogroup mqx_sdk_config_module mqx_sdk_config module documentation
**  @{
*/         
#ifndef __mqx_sdk_config_h__
#define __mqx_sdk_config_h__

/* Configuration options */
#define MQX_LITE_CONFIG                1
#define MQX_SMALL_RAM_CONFIG           2
#define MQX_MAXIMUM_CONFIG             3

/* Select initial MQX configuration */
#define MQX_COMMON_CONFIG              MQX_SMALL_RAM_CONFIG

#define CMSIS_ENABLED                  1

#define MQX_USE_IDLE_TASK              1
#define MQXCFG_ENABLE_FP               0
#define MQX_TASK_DESTRUCTION           1

/* Mandatory CPU identification */
#ifndef MQX_CPU
  #define MQX_CPU                      PSP_CPU_NUM(PSP_CPU_ARCH_ARM_CORTEX_M4, PSP_CPU_GROUP_KINETIS_K6X, 0)
#endif

#ifndef MQX_USE_IO_OLD
  #define MQX_USE_IO_OLD 0
#elif MQX_USE_IO_OLD
  #error This version of MQX does not support legacy IO (FIO). Please force MQX_USE_IO_OLD to 0 in user_config.h
#endif

/*
** When MQX_USE_LWEVENT is defined as 1,
** then mqx will compile in the support code for light weight events.
*/
#ifndef MQX_USE_LWEVENT
  #define MQX_USE_LWEVENT              1
#endif

/*
** When MQX_USE_LWMSGQ is defined as 1,
** then mqx will compile in the support code for light weight message queues.
*/
#ifndef MQX_USE_LWMSGQ
  #define MQX_USE_LWMSGQ               1
#endif

/*
** When MQX_USE_MESSAGES is defined as 1,
** then mqx will compile in the support code for messages.
*/
#ifndef MQX_USE_MESSAGES
  #define MQX_USE_MESSAGES             1
#endif

/*
** When MQX_USE_MUTEXES is defined as 1,
** then mqx will compile in the support code for mutex component.
*/
#ifndef MQX_USE_MUTEXES
  #define MQX_USE_MUTEXES              1
#endif

/*
** When MQX_USE_TIMER is defined as 1,
** then mqx will compile in the support code for timer component.
*/
#ifndef MQX_USE_TIMER
  #define MQX_USE_TIMER                1
#endif

/*
** Lowest task priority value.
*/
#ifndef MQXCFG_LOWEST_TASK_PRIORITY
  #define MQXCFG_LOWEST_TASK_PRIORITY  32
#endif

/* Additional settings can be defined in the property User Definitions > Definitions of the MQX RTOS component */
  

/* Select MQX configurations according to project settings. */
#include "small_ram_config.h"

/* In the case the I/O subsystem is not used,
** the printf should not be taken from stdlib (dependent on NIO)
** but rather use SDK's debug_printf implementation.
*/
#if BSPCFG_ENABLE_IO_SUBSYSTEM == 0
  #define printf                       debug_printf
#endif  

/* Enable verification checks in kernel */
#include "verif_enabled_config.h"

#endif /* __mqx_sdk_config_h__ */

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
