/**
 * Pinout connections:
 * --------------------
 * 25LC160 EEPROM    STM32VL Eval Board
 * ---------------- ------------------
 *    CS     ------->    PC10
 *    SO     ------->    PB14 (MISO)
 *    WP     ------->    -
 *    GND    ------->    GND
 *    VCC    ------->    3.3V
 *    HOLD   ------->    GND
 *    SCK    ------->    PB13 (SCK)
 *    SI     ------->    PB15 (MOSI)
 */

#include "main.h"
#include "spi.h"
#include "eeprom.h"
#define USE_FULL_ASSERT 1

static void MX_GPIO_Init(void);
void SystemClock_Config(void);

SPI_HandleTypeDef EEPROM_SPI;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  EEPROM_Init();

  uint8_t dtx = 0xff;
  uint8_t drx = 0xff;

  // send dummy data on tx line to initialise clock on clck line
  HAL_SPI_TransmitReceive(&EEPROM_SPI, &dtx, &drx, 2, 100);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  HAL_Delay(1000);

  EEPROM_ReadStatus(&EEPROM_SPI);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  EEPROM_WriteEnable(&EEPROM_SPI);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  EEPROM_ReadStatus(&EEPROM_SPI);


  while (1)
  {

  }
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

  /*Configure GPIO pins : PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_SET);
}


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

#ifdef USE_FULL_ASSERT
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
