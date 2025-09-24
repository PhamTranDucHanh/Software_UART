/*
 * UART_Sender.c
 *
 *  Created on: Sep 24, 2025
 *      Author: HANH
 */

#include "UART_Sender.h"


state currentState = START;

int count = 0;
int complete = 0;
int uart_send_byte(uint8_t msg, int enable){
	if (!enable) return 2;
	switch (currentState){
		case START:
			HAL_GPIO_WritePin(GPIOA, TX_Pin, 0);
			complete = 0;

			currentState = DATA;
			break;
		case DATA:
			HAL_GPIO_WritePin(GPIOA, TX_Pin, msg & (1 << count));
			count++;

			if (count == 8){
				count = 0;
				currentState = END;
			}
			break;
		case END:
			HAL_GPIO_WritePin(GPIOA, TX_Pin, 0);
			complete = 1;
			currentState = START;
			break;
		default:
			break;
	}
	return complete;

}
