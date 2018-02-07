/*
 * handler.h
 *
 *  Created on: Feb 6, 2018
 *      Author: lymacasm
 */

#ifndef SOURCES_UART_HANDLER_H_
#define SOURCES_UART_HANDLER_H_

#include <stdbool.h>
#include <mqx.h>
#include <message.h>

extern bool OpenR(uint16_t stream_no);
extern _queue_id OpenW();
extern bool _putline(_queue_id qid, const char* string);
extern bool _get_line(char* string);
extern bool Close();



#endif /* SOURCES_UART_HANDLER_H_ */
