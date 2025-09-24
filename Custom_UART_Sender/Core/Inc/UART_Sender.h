/*
 * UART_Sender.h
 *
 *  Created on: Sep 24, 2025
 *      Author: HANH
 */

#ifndef INC_UART_SENDER_H_
#define INC_UART_SENDER_H_

#include "main.h"
#define send_complete	1
typedef enum{
	START,
	DATA,
	END,
	NONE
} state;

int uart_send_byte(uint8_t msg, int enable);

#endif /* INC_UART_SENDER_H_ */
