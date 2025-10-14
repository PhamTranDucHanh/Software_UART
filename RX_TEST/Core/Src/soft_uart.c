/*
 * soft_uart.c
 *
 *  Created on: Oct 14, 2025
 *      Author: my pc
 */
#include "soft_uart.h"
#include "lcd.h"

// Thời gian trễ cho 1 bit (tính bằng micro giây)
// 1/9600 * 1,000,000 us/s ~= 104 us
#define BIT_TIME_US 104

// Hàm delay micro giây sử dụng Timer
void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim2, 0); // Reset bộ đếm
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

// Khởi tạo
void soft_uart_init() {
    // Bắt đầu timer
    HAL_TIM_Base_Start(&htim2);
    // Đảm bảo chân TX ở trạng thái nghỉ (mức cao)
    HAL_GPIO_WritePin(SW_UART_TX_PORT, SW_UART_TX_PIN, GPIO_PIN_SET);
}

// Hàm gửi 1 byte
void soft_uart_transmit_byte(uint8_t data) {
    // -- START BIT --
    HAL_GPIO_WritePin(SW_UART_TX_PORT, SW_UART_TX_PIN, GPIO_PIN_RESET);
    delay_us(BIT_TIME_US);

    // -- DATA BITS (LSB first) --
    for (int i = 0; i < 8; i++) {
        if ((data >> i) & 0x01) { // Nếu bit thứ i là 1
            HAL_GPIO_WritePin(SW_UART_TX_PORT, SW_UART_TX_PIN, GPIO_PIN_SET);
        } else { // Nếu bit thứ i là 0
            HAL_GPIO_WritePin(SW_UART_TX_PORT, SW_UART_TX_PIN, GPIO_PIN_RESET);
        }
        delay_us(BIT_TIME_US);
    }

    // -- STOP BIT --
    HAL_GPIO_WritePin(SW_UART_TX_PORT, SW_UART_TX_PIN, GPIO_PIN_SET);
    delay_us(BIT_TIME_US);
}

// Hàm nhận 1 byte
uint8_t soft_uart_receive_byte() {
    uint8_t received_data = 0;
    //lcd_ShowStr(10, 150, "TEST", YELLOW, BLACK, 24, 0);
    // 1. Chờ đợi Start bit (đường truyền bị kéo xuống thấp)
    while (HAL_GPIO_ReadPin(SW_UART_RX_PORT, SW_UART_RX_PIN) == GPIO_PIN_SET);
    //lcd_ShowStr(10, 190, "TEST2", YELLOW, BLACK, 24, 0);
    // 2. Đồng bộ hóa: Khi phát hiện Start bit, chờ 1 nửa bit time
    // để dịch chuyển đến điểm giữa của bit đầu tiên.
    delay_us(BIT_TIME_US / 2);

    // 3. Đọc 8 bit dữ liệu
    for (int i = 0; i < 8; i++) {
        delay_us(BIT_TIME_US); // Chờ 1 bit time để đến bit tiếp theo
        if (HAL_GPIO_ReadPin(SW_UART_RX_PORT, SW_UART_RX_PIN) == GPIO_PIN_SET) {
            received_data |= (1 << i);
        }
    }

    // 4. Chờ Stop bit
    delay_us(BIT_TIME_US);

    return received_data;
}

