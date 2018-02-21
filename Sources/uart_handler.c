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

	// ask for read priv command to handler (user queue)
	// wait for the reply
	// check status
	// return based on status

	return FALSE;
}

extern _queue_id OpenW()

{
	// ask for write priv from handler
	// wait for reply
	// check status
	// return based on status
	return 0;
}

extern bool _putline(_queue_id qid, const char* string)
{
	// send WRITE request and DATA(ending with /0) to write.
	// handler reply success or failure
	return FALSE;
}

extern bool _get_line(char* string)
{
	// send 'READ' req
	// handler checks if task_id have priv open (OpenR  not closed yet)
	// check if its's success
	// read first two bytes of data (i.e. queue id) sent from handler
	// grab the first message from the queue
	// assign it to string pointer
	// return success t or f
	return FALSE;
}

extern bool Close()
{
	return FALSE;
}
