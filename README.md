# Software UART Implementation

## Overview
Software UART (Soft UART) is a technique to emulate UART communication using GPIO pins and timers instead of relying on hardware UART peripherals. This approach is based on **bit-banging**, where individual bits of data are manually controlled and read through GPIO pins at precise time intervals.

---

## Bit-banging Technique
### Transmitting Data
- Each byte is transmitted bit by bit through the TX pin.
- The transmission starts with a **Start bit** (logic low), followed by 8 **Data bits** (Least Significant Bit first), and ends with a **Stop bit** (logic high).
- A timer is used to create precise delays between bits, ensuring the correct baud rate.

### Receiving Data
- Synchronization begins with the **Start bit** (logic low) detected on the RX pin.
- Each subsequent bit is sampled at the middle of its time interval using a timer to ensure accuracy.
- The reception ends with a **Stop bit** to validate the integrity of the received byte.

---

## Timing and Synchronization
### Bit Timing
- The delay between bits is calculated based on the baud rate. For example, at 9600 baud, each bit lasts approximately 104 microseconds.
- A timer is configured to ensure precise timing, avoiding signal distortion or data corruption.

### Packet Structure
A complete packet consists of:
1. **Start Byte**: Indicates the beginning of the packet.
2. **Command Byte**: Specifies the operation or instruction.
3. **Length Byte**: Defines the number of data bytes in the packet.
4. **Data Bytes**: Contains the actual payload.
5. **Checksum Byte**: Ensures data integrity by verifying the sum of all previous bytes.

Each byte in the packet is transmitted and received sequentially, maintaining synchronization between the sender (Master) and receiver (Slave).

---

## Implementation Details
### Transmitter (Master)
- The Master sends packets using the `soft_uart_transmit_byte()` function, which handles bit-by-bit transmission with precise timing.
- A GPIO pin (`ENABLE`) is used to signal the Slave to prepare for receiving data.
- The Master ensures the integrity of the packet by appending a checksum byte.

### Receiver (Slave)

The Slave operates in an interrupt-driven manner to efficiently handle incoming packets. A dedicated GPIO pin (`ENABLE`) is configured to trigger an external interrupt (EXTI) when its signal transitions to a falling edge (logic low). This mechanism ensures that the Slave begins processing only when the Master signals readiness to send data.

#### Interrupt Handling
- When the `ENABLE` pin transitions to a falling edge, the external interrupt handler (`HAL_GPIO_EXTI_Callback`) is invoked.
- Inside the interrupt handler, the Slave prepares to receive the packet by calling the `soft_uart_receive_byte()` function to read the incoming data byte by byte.
- The interrupt-driven approach minimizes CPU usage and ensures timely response to the Master’s signal.
---

## Applications
- Ideal for systems without hardware UART peripherals or when custom GPIO-based UART communication is required.
- Enables flexible UART communication on non-standard pins, making it suitable for embedded systems with limited resources.

---

## Advantages
- **Flexibility**: Can be implemented on any GPIO pins.
- **Portability**: Works on microcontrollers without dedicated UART hardware.
- **Customizability**: Packet structure and timing can be tailored to specific application needs.

---

## Limitations
- **CPU Usage**: Bit-banging requires precise timing and can block the CPU during transmission/reception.
- **Baud Rate**: Limited by the timer resolution and system clock speed.
- **Error Handling**: Requires robust checksum validation to ensure data integrity

---

## Demo

### Images
<img width="780" height="410" alt="image" src="https://github.com/user-attachments/assets/00590e20-44e8-4436-82bd-951157026024" />


<img width="780" height="410" alt="image" src="https://github.com/user-attachments/assets/3d26dab1-fa56-45ae-a4cd-f7da12feb171" />



### Videos (Google drive link)


---

For detailed implementation, refer to the functions `soft_uart_transmit_byte()` and `soft_uart_receive_byte()` in the source code.
