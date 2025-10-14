/*
 * UART_Sender.c (ĐÃ SỬA LỖI)
 *
 *  Created on: Sep 24, 2025
 *      Author: HANH, Duong(fixed)
 */

#include "UART_Sender.h"

// Biến static để chúng chỉ thuộc về file này
static state currentState = START;
static int bit_count = 0;

int uart_send_byte(uint8_t msg, int enable){
	if (!enable) return 2; // Nếu không được phép gửi thì thoát

	switch (currentState){
		case START:
			// Bắt đầu: Kéo chân TX xuống thấp để gửi Start Bit
			HAL_GPIO_WritePin(GPIOA, TX_Pin, 0);

			// Chuyển sang trạng thái gửi dữ liệu
			currentState = DATA;
			break;

		case DATA:
			// LẤY RA TỪNG BIT VÀ GỬI ĐI (ĐÃ SỬA LỖI)
			// Dịch phải `msg` `bit_count` lần, sau đó AND với 1 để lấy ra bit cuối cùng
			if ((msg >> bit_count) & 0x01) {
				HAL_GPIO_WritePin(GPIOA, TX_Pin, 1); // Nếu bit là 1
			} else {
				HAL_GPIO_WritePin(GPIOA, TX_Pin, 0); // Nếu bit là 0
			}

			bit_count++; // Chuyển sang bit tiếp theo

			if (bit_count >= 8){ // Nếu đã gửi đủ 8 bit
				bit_count = 0; // Reset bộ đếm bit cho lần sau
				currentState = END; // Chuyển sang trạng thái kết thúc
			}
			break;

		case END:
			// Kết thúc: Kéo chân TX lên cao để gửi Stop Bit (ĐÃ SỬA LỖI)
			HAL_GPIO_WritePin(GPIOA, TX_Pin, 1);

			// Quay về trạng thái ban đầu để sẵn sàng cho byte tiếp theo
			currentState = START;
			return 1; // Trả về 1, báo hiệu đã gửi xong 1 byte
			break;

		default:
			break;
	}
	return 0; // Trả về 0, báo hiệu đang trong quá trình gửi
}
