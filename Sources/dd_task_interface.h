/*
 * edf_task_interface.h
 *
 *  Created on: Mar 13, 2018
 *      Author: lymacasm
 */

#ifndef SOURCES_DD_TASK_INTERFACE_H_
#define SOURCES_DD_TASK_INTERFACE_H_

#include <stdio.h>
#include <mutex.h>

/* Structs for task lists. Doubly linked list for easy sorting */
struct task_list {
	_task_id tid;
	time_t deadline;
	uint32_t task_type;
	time_t creation_time;
	struct task_list_node* next_cell;
	struct task_list_node* previous_cell;
};

extern MUTEX_STRUCT scheduler_mutex;
extern MUTEX_STRUCT overhead_mutex;

void 	 dd_init();
_task_id dd_tcreate(uint32_t template_index, uint32_t task_param, time_t deadline);
_task_id dd_delete(_task_id task_id);
uint32_t dd_return_active_list(struct task_list ** list);
uint32_t dd_return_overdue_list(struct task_list ** list);
uint32_t dd_return_overhead(time_t * ovrhd);
uint32_t dd_reset_scheduler();

#endif /* SOURCES_DD_TASK_INTERFACE_H_ */
