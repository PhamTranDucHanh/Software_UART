/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h" // Thư viện timer tự chế của bạn
#include "UART_Sender.h"    // Thư viện gửi UART tự chế của bạn

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// �?ịnh nghĩa các trạng thái cho máy trạng thái trong hàm main()
// Giúp code dễ đ�?c và dễ hiểu hơn là dùng số 0 và 1.
enum {
	STATE_IDLE,    // Trạng thái nghỉ, ch�? để gửi byte tiếp theo
	STATE_SENDING  // Trạng thái đang bận rộn gửi các bit của 1 byte
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_DURATION_MS   1000   // Th�?i gian gửi 1 bit (500ms để debug)
#define INTER_BYTE_DELAY_MS 15000  // Th�?i gian nghỉ giữa 2 byte (2 giây)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
// Biến toàn cục để lưu trạng thái hiện tại của chương trình
int main_task_state = STATE_IDLE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  �?ây là hàm callback của Timer phần cứng (TIM2).
  * @note   Nó sẽ tự động được g�?i mỗi khi TIM2 đếm xong (được cấu hình là 1ms).
  *         �?ây chính là "nhịp đập" trái tim, cung cấp năng lượng cho hệ thống
  *         software_timer mà bạn đã tự viết.
  * @param  htim: con tr�? tới timer đã gây ra ngắt.
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    timerRun();
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  // Biến để lưu giá trị số cần gửi, bắt đầu từ 0
  uint8_t msg_to_send = 5;

  // Biến để nhận kết quả trả v�? từ hàm uart_send_byte()
  int sending_status = 0;

  // Thiết lập timer số 0 để đi�?u khiển th�?i gian gửi mỗi bit
  timerSet(0, BIT_DURATION_MS);

  // Thiết lập timer số 1 để tạo khoảng nghỉ giữa các byte
  timerSet(1, INTER_BYTE_DELAY_MS);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //uint8_t msg = 0;
  //timerSet (0, 6);
  //timerSet (1, 1000);
  //int enable = 1;
  //int complete = 0;
  //timerSet (0, 500);
  while (1)
  {

//	  if (timerFlag(0)){
//		  HAL_GPIO_TogglePin(GPIOA, TX_Pin);
//	  }
//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
//	  HAL_Delay(1000);








    // Cấu trúc switch-case để quản lý các trạng thái của chương trình
    switch (main_task_state)
    {
      case STATE_IDLE:
        // Ở trạng thái nghỉ, chúng ta chỉ cần kiểm tra timer 1 (timer nghỉ giữa các byte)
        if (timerFlag(1)) {
          // Timer 1 đã đếm xong! �?ã đến lúc gửi byte tiếp theo.

          // 1. Chuyển sang trạng thái đang gửi
          main_task_state = STATE_SENDING;

          // 2. G�?i hàm gửi lần đầu tiên để gửi ngay lập tức bit đầu tiên (Start Bit)
          //    Tham số thứ hai là '1' để cho phép (enable) việc gửi.
          uart_send_byte(msg_to_send, 1);

          // 3. Sau khi gửi start bit, cài đặt lại timer 0 để 500ms sau sẽ gửi bit tiếp theo
          timerSet(0, BIT_DURATION_MS);
        }
        break;

      case STATE_SENDING:
        // Ở trạng thái đang gửi, chúng ta kiểm tra timer 0 (timer giữa các bit)
        if (timerFlag(0)) {
          // �?ã hết 500ms, đã đến lúc gửi bit tiếp theo.

          // 1. Cài đặt lại timer 0 cho 500ms tiếp theo ngay lập tức
          //timerSet(0, BIT_DURATION_MS);
        	HAL_Delay(1000);

          // 2. G�?i hàm gửi bit và lưu lại kết quả trả v�?
          sending_status = uart_send_byte(msg_to_send, 1);

          // 3. Kiểm tra kết quả
          if (sending_status == 1) {
            // Nếu hàm trả v�? 1, có nghĩa là đã gửi xong toàn bộ byte (bao gồm cả Stop Bit).
            // Công việc hoàn thành, đã đến lúc nghỉ ngơi.

            // a. Chuyển v�? trạng thái nghỉ
            main_task_state = STATE_IDLE;

            // b. Tăng giá trị của số cần gửi lên 1 cho lần sau
            msg_to_send = 5;

            // c. Cài đặt lại timer 1 để bắt đầu đếm 2 giây nghỉ
            //timerSet(1, INTER_BYTE_DELAY_MS);
            HAL_Delay(15000);
          }
        }
        break;

      default:
        // Trư�?ng hợp phòng h�? nếu biến trạng thái bị lỗi
        main_task_state = STATE_IDLE;
        break;
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 799;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TX_GPIO_Port, TX_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RX_Pin */
  GPIO_InitStruct.Pin = RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TX_Pin */
  GPIO_InitStruct.Pin = TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TX_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
