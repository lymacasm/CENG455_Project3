/* ###################################################################
**     Filename    : os_tasks.c
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
**         handler_task - void handler_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <stdio.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */

/* Message queues */
#define READ_QUEUE 15

/* Data sizes */
#define RX_BUF_SIZE (DATA_SIZE - 1)
#define TX_BUF_SIZE DATA_SIZE

typedef struct rw_privileges
{
	_queue_id QUEUE_ID;
	_task_id TASK_ID;
	struct rw_privileges * NEXT;
} RW_PRIVILEGES, * RW_PRIVILEGES_PTR;

_pool_id rx_msg_pool;
_pool_id user_msg_pool;
volatile uint8_t user_pool_created = 0;

static _queue_id find_privilege(RW_PRIVILEGES_PTR priv_head, _task_id tid)
{
	RW_PRIVILEGES_PTR priv_searcher = priv_head;
	while(priv_searcher != NULL)
	{
		if(priv_searcher->TASK_ID == tid)
			return priv_searcher->QUEUE_ID;
		priv_searcher = priv_searcher->NEXT;
	}
	return 0;
}

static _status add_privilege(RW_PRIVILEGES_PTR privilege_head, _task_id tid, _queue_id qid)
{
	RW_PRIVILEGES_PTR new_privilege;
	new_privilege = _mem_alloc(sizeof(RW_PRIVILEGES));
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to allocate memory for privilege list.\n");
		printf("Error code: %x\n", MQX_OK);
		_task_set_error(MQX_OK);
		return FAILURE;
	}
	new_privilege->QUEUE_ID = qid;
	new_privilege->TASK_ID = tid;
	new_privilege->NEXT = privilege_head;
	privilege_head = new_privilege;
	return SUCCESS;
}

static _status remove_privilege(RW_PRIVILEGES_PTR privilege_head, _task_id tid)
{
	RW_PRIVILEGES_PTR priv_searcher = privilege_head;

	/* Check for empty list */
	if(privilege_head == NULL)
	{
		printf("Tried to remove privilege from empty list.\n");
		return FAILURE;
	}

	/* Check if the privilege to remove is the first list item */
	if(priv_searcher->TASK_ID == tid)
	{
		privilege_head = priv_searcher->NEXT;
		_mem_free(priv_searcher);
		if(_task_get_error() != MQX_OK)
		{
			printf("Failed to free memory from privilege list.\n");
			printf("Error code: %x\n", MQX_OK);
			_task_set_error(MQX_OK);
			return FAILURE;
		}
		return SUCCESS;
	}

	/* Search through the rest of the list */
	while(priv_searcher->NEXT != NULL)
	{
		if(priv_searcher->TASK_ID == tid)
		{
			RW_PRIVILEGES_PTR tmp = priv_searcher->NEXT;
			priv_searcher->NEXT = priv_searcher->NEXT->NEXT;
			_mem_free(tmp);
			if(_task_get_error() != MQX_OK)
			{
				printf("Failed to free memory from privilege list.\n");
				printf("Error code: %x\n", MQX_OK);
				_task_set_error(MQX_OK);
				return FAILURE;
			}
			return SUCCESS;
		}

		priv_searcher = priv_searcher->NEXT;
	}
	return FAILURE;
}

/*
** ===================================================================
**     Callback    : handler_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void handler_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
	RW_PRIVILEGES_PTR read_privs_head = NULL;
	RW_PRIVILEGES_PTR write_privs_head = NULL;
	RX_MESSAGE_PTR rx_msg_ptr = NULL;
	USER_MESSAGE_PTR user_msg_ptr = NULL;
	_queue_id rx_qid;
	_queue_id user_qid;
	char rx_buf[RX_BUF_SIZE];
	char tx_buf[TX_BUF_SIZE];
	uint8_t rx_buf_idx = 0;
	uint8_t tx_len = 0;

	printf("handlerTask Created!\n\r");

	char buf[13];
	sprintf(buf, "\n\rType here: ");
	UART_DRV_SendDataBlocking(myUART_IDX, (uint8_t*)buf, sizeof(buf), 1000);

	/* Open RX ISR message queue */
	rx_qid = _msgq_open(RX_QUEUE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open RX ISR message queue.\n");
		printf("Error code: %x\n", MQX_OK);
		exit(1);
	}

	/* Create RX ISR message pool */
	rx_msg_pool = _msgpool_create(sizeof(RX_MESSAGE), RX_QUEUE_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open RX ISR message pool.\n");
		printf("Error code: %x\n", MQX_OK);
		exit(1);
	}

	/* Open User message queue */
	user_qid = _msgq_open(USER_QUEUE, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open User message queue.\n");
		printf("Error code: %x\n", MQX_OK);
		exit(1);
	}

	/* Create User message pool */
	user_msg_pool = _msgpool_create(sizeof(USER_MESSAGE), USER_QUEUE_SIZE, 0, 0);
	if(_task_get_error() != MQX_OK)
	{
		printf("Failed to open User message pool.\n");
		printf("Error code: %x\n", MQX_OK);
		exit(1);
	}

	user_pool_created = 1;

#ifdef PEX_USE_RTOS
	while(TRUE)
#endif
	{
		uint16_t rx_msg_count = 0;
		uint16_t user_msg_count = 0;

		/* Get the message counts */
		rx_msg_count = _msgq_get_count(rx_qid);
		if(rx_msg_count == 0 && _task_get_error() != MQX_OK)
		{
			printf("Failed to check pending RX ISR message count.\n");
			printf("Error code: %d\n", _task_get_error());
			_task_set_error(MQX_OK);
		}

		user_msg_count = _msgq_get_count(user_qid);
		if(user_msg_count == 0 && _task_get_error() != MQX_OK)
		{
			printf("Failed to check pending User message count.\n");
			printf("Error code: %d\n", _task_get_error());
			_task_set_error(MQX_OK);
		}

		/* This section of code handles data coming in from the ISR
		 * It checks for special command characters, and makes sure
		 * that all characters entered are keyboard characters.
		 * Data coming in will be echoed to the terminal.
		 */
		if(rx_msg_count > 0)
		{
			rx_msg_ptr = _msgq_receive(rx_qid, 0);
			if(_task_get_error() != MQX_OK)
			{
				printf("Failed to receive RX ISR message.\n");
				printf("Error code: %x\n", MQX_OK);
				_task_set_error(MQX_OK);
			}

			/* Process received data */
			else
			{
				/* Check for \b (backspace) */
				if(rx_msg_ptr->DATA == '\b')
				{
					if(rx_buf_idx > 0) --rx_buf_idx;
					tx_buf[0] = '\b'; // Backspace
					tx_buf[1] = ' '; // Space
					tx_buf[2] = '\b'; // Backspace
					tx_len = 3;
				}
				/* Check for newline character and send data to listeners */
				else if(rx_msg_ptr->DATA == '\r' || rx_msg_ptr->DATA == '\n')
				{
					RW_PRIVILEGES_PTR priv_searcher = read_privs_head;
					_queue_id handler_qid;

					/* Setup tx */
					tx_buf[0] = 0x1B; // ESCAPE SEQUENCE
					tx_buf[1] = 'E';
					tx_len = 2;

					/* Open message queue */
					handler_qid = _msgq_open(READ_QUEUE, 0);
					if(_task_get_error() != MQX_OK)
					{
						printf("Failed to open the read message queue.\n");
						printf("Error code: %d\n", _task_get_error());
						_task_set_error(MQX_OK);
					}

					/* Send data back to all tasks with read privileges */
					while(priv_searcher != NULL)
					{
						USER_MESSAGE_PTR read_msg;
						_queue_id target_qid;

						/* Grab the target qid */
						target_qid = priv_searcher->QUEUE_ID;

						/* Allocate message */
						read_msg = (USER_MESSAGE_PTR) _msg_alloc(user_msg_pool);
						if(read_msg == NULL)
						{
							printf("Could not allocate a message for read broadcast\n");
							continue;
						}

						read_msg->HEADER.SOURCE_QID = handler_qid;
						read_msg->HEADER.TARGET_QID = target_qid;
						read_msg->HEADER.SIZE = sizeof(USER_MESSAGE);
						read_msg->CMD_ID = READ;

						/* TODO: NULL terminate data */
						memcpy(read_msg->DATA, rx_buf, rx_buf_idx);

						read_msg->DATA[rx_buf_idx] = '\0';

						/* Send message */
						_msgq_send(read_msg);
						if(_task_get_error() != MQX_OK)
						{
							printf("Failed to send data through the read message queue.\n");
							printf("Error code: %d\n", _task_get_error());
							_task_set_error(MQX_OK);
						}

						/* Go to next element */
						priv_searcher = priv_searcher->NEXT;
					}

					/* Close message queue */
					_msgq_close(handler_qid);
					if(_task_get_error() != MQX_OK)
					{
						printf("Failed to close the read message queue.\n");
						printf("Error code: %d\n", _task_get_error());
						_task_set_error(MQX_OK);
					}

					/* Reset rx buffer */
					rx_buf_idx = 0;
				}
				/* Check for erase line command */
				else if(rx_msg_ptr->DATA == 0x15)
				{
					rx_buf_idx = 0;

					/* Delete line */
					tx_buf[0] = 0x1B; // ESCAPE SEQUENCE
					tx_buf[1] = '[';
					tx_buf[2] = '2';
					tx_buf[3] = 'K';

					/* Next line */
					tx_buf[4] = 0x1B; // ESCAPE SEQUENCE
					tx_buf[5] = 'E';

					/* Back up to last line */
					tx_buf[6] = 0x1B;
					tx_buf[7] = '[';
					tx_buf[8] = 0x1;
					tx_buf[9] = 'A';
					tx_len = 10;
				}
				/* Check for erase word command */
				else if(rx_msg_ptr->DATA == 0x17)
				{
					uint8_t i;
					uint8_t last_space = 0;
					for(i = 0; i < rx_buf_idx; i++)
					{
						if(rx_buf[i] == ' ') last_space = i;
					}

					/* Move cursor to the last_space position */
					tx_buf[0] = 0x1B; // ESCAPE SEQUENCE
					tx_buf[1] = '[';
					tx_buf[2] = ((i - last_space) & 0xFF);
					tx_buf[3] = 'D';

					/* Clear line from cursor right */
					tx_buf[4] = 0x1B; // ESCAPE SEQUENCE
					tx_buf[5] = '[';
					tx_buf[6] = 'K';
					tx_len = 7;

					rx_buf_idx = last_space;
				}
				/* TODO: Add a bunch of else if's for each command */
				else if(isprint(rx_msg_ptr->DATA) && rx_buf_idx < RX_BUF_SIZE)
				{
					rx_buf[rx_buf_idx] = rx_msg_ptr->DATA;
					rx_buf_idx++;
					tx_buf[0] = rx_msg_ptr->DATA;
					tx_len = 1;
				}

				/* Free message */
				_msg_free(rx_msg_ptr);
			}

			/* Transmit data back through UART */
			if(tx_len > 0)
			{
				char buf_cpy[TX_BUF_SIZE];
				uint32_t bytes_remaining = 0;
				while(UART_DRV_GetTransmitStatus(myUART_IDX, &bytes_remaining) != kStatus_UART_Success);
				memcpy(buf_cpy, tx_buf, tx_len);
				UART_DRV_SendData(myUART_IDX, (uint8_t*)buf_cpy, tx_len);
				tx_len = 0;
			}
		} /* End processing of RX ISR requests */

		/* Deal with user requests */
		if(user_msg_count > 0)
		{
			user_msg_ptr = _msgq_receive(user_qid, 0);
			if(_task_get_error() != MQX_OK)
			{
				printf("Failed to receive User message.\n");
				printf("Error code: %x\n", MQX_OK);
				_task_set_error(MQX_OK);
			}

			/* Process user request */
			else
			{
				/* Switch destination and source */
				user_msg_ptr->HEADER.TARGET_QID = user_msg_ptr->HEADER.SOURCE_QID;
				user_msg_ptr->HEADER.SOURCE_QID = user_qid;

				/* Check the command id */
				switch(user_msg_ptr->CMD_ID)
				{
				case (READ_PRIV):
					user_msg_ptr->CMD_ID = READ_PRIV_ACK;
					/* Make sure we don't already have a privilege for this task */
					if(find_privilege(read_privs_head, user_msg_ptr->TASK_ID) != 0)
					{
						user_msg_ptr->STATUS = FAILURE;
					}
					/* Add read privilege */
					else
					{
						/* Get the queue id of the sending task */
						/* ASSUMPTION : First two bytes of data contain the queue id, in Big Endian format */
						_queue_id qid = (user_msg_ptr->DATA[0] << 8) | user_msg_ptr->DATA[1];
						user_msg_ptr->STATUS = add_privilege(read_privs_head,
								user_msg_ptr->TASK_ID, qid);
					}
					break;
				case (WRITE_PRIV):
					user_msg_ptr->CMD_ID = WRITE_PRIV_ACK;
					if(write_privs_head == NULL)
					{
						user_msg_ptr->STATUS = add_privilege(write_privs_head,
								user_msg_ptr->TASK_ID, 0xFFFF);
					}
					else
					{
						user_msg_ptr->STATUS = FAILURE;
					}
					break;
				case (CLOSE):
					user_msg_ptr->CMD_ID = CLOSE_ACK;
					/* If either of the read or write removals are successful, the entire operation is a success (OR) */
					user_msg_ptr->STATUS = remove_privilege(read_privs_head,
							user_msg_ptr->TASK_ID);
					user_msg_ptr->STATUS |= remove_privilege(write_privs_head,
							user_msg_ptr->TASK_ID);
					break;
				case (READ):
					user_msg_ptr->CMD_ID = READ_ACK;
					_queue_id destination = find_privilege(read_privs_head,
							user_msg_ptr->TASK_ID);
					if(destination != 0)
					{
						user_msg_ptr->DATA[0] = (destination & 0xFF00) >> 8;
						user_msg_ptr->DATA[1] = destination & 0xFF;
						user_msg_ptr->STATUS = SUCCESS;
					}
					else
					{
						user_msg_ptr->STATUS = FAILURE;
					}
					break;
				case (WRITE):
					user_msg_ptr->CMD_ID = WRITE_ACK;
					user_msg_ptr->STATUS = FAILURE;
					/* Check if task has write privilege */
					if(find_privilege(write_privs_head, user_msg_ptr->TASK_ID) != 0)
					{
						/* Wait for TX to finish */
						uint8_t buf_cpy[TX_BUF_SIZE];
						uint32_t bytes_remaining = 0;
						while(UART_DRV_GetTransmitStatus(myUART_IDX, &bytes_remaining) != kStatus_UART_Success);
						strcpy((char*)buf_cpy, (char*)user_msg_ptr->DATA);
						UART_DRV_SendData(myUART_IDX, (uint8_t*)buf_cpy, strlen((char*)buf_cpy));
						user_msg_ptr->STATUS = SUCCESS;
					}
					break;
				default:
					/* Not a valid command */
					user_msg_ptr->CMD_ID = UNKNOWN_ACK;
					user_msg_ptr->STATUS = FAILURE;
				}
				/* Send response */
				_msgq_send(user_msg_ptr);
			}
		}
		OSA_TimeDelay(10);                 /* Example code (for task release) */
	}
}

/* END os_tasks */

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
