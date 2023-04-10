#include "main.h"
#include "uart.h"
#include <string.h>

static void MX_GPIO_Init(void);

UART_HandleTypeDef huart;
uint8_t buffer[10];

int uart_open(USART_TypeDef *USARTx, uint32_t baud)
{
  if (USARTx == USART1)
  {
    huart.Instance = USART1;
  }
  else if (USARTx == USART2)
  {
    huart.Instance = USART2;
  }
  else if (USARTx == USART3)
  {
    huart.Instance = USART3;
  }
  huart.Init.BaudRate = baud;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_NONE;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
  }

  return 1;
};

int uart_close(UART_HandleTypeDef *huart)
{
  HAL_UART_DeInit(huart);
  return 1;
}

int uart_main()
{
    HAL_Init();

    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    uart_open(USART1, 115200);

  char *message = "Hello World!\n\r"; 
    while (1)
    {
      	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
          HAL_UART_Transmit(&huart, (uint8_t *) message, strlen(message), 100);
          HAL_Delay(100);

	    }
        // HAL_UART_Transmit(&huart, (uint8_t *)message, strlen(message), 100);
        // HAL_Delay(250);

        // if (HAL_UART_Receive(&huart, buffer, 10, HAL_MAX_DELAY) != HAL_OK)
        // {
        //   // ERROR
        // };

        // HAL_UART_Transmit(&huart, (uint8_t *) buffer, sizeof(buffer), 100);
        

    }
    return 1;
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}