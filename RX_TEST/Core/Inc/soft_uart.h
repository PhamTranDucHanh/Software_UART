/*
 * soft_uart.h
 *
 *  Created on: Oct 14, 2025
 *      Author: DuongGiauTen
 *      Modifier: Duc Hanh
 */

#ifndef INC_SOFT_UART_H_
#define INC_SOFT_UART_H_

#include "main.h"

// Định nghĩa chân và cổng cho TX và RX
#define SW_UART_TX_PORT GPIOA
#define SW_UART_TX_PIN  GPIO_PIN_4
#define SW_UART_RX_PORT GPIOA
#define SW_UART_RX_PIN  GPIO_PIN_5

#define START_BYTE 0xAA
#define CMD_DISPLAY_TEXT 0x01

// Khai báo timer bạn đang dùng
extern TIM_HandleTypeDef htim2;

void soft_uart_init();
void soft_uart_transmit_byte(uint8_t data);
uint8_t soft_uart_receive_byte();
void master_send_string(const char* str);

#endif /* INC_SOFT_UART_H_ */
