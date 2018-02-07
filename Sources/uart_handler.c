/*
 * handler.c
 *
 *  Created on: Feb 6, 2018
 *      Author: lymacasm
 */

#include "uart_handler.h"
#include "os_tasks.h"

extern bool OpenR(uint16_t stream_no)
{
	return FALSE;
}

extern _queue_id OpenW()
{
	return 0;
}

extern bool _putline(_queue_id qid, const char* string)
{
	return FALSE;
}

extern bool _get_line(char* string)
{
	return FALSE;
}

extern bool Close()
{
	return FALSE;
}
