

#include "main.h"
#include "spi.h"

static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);

SPI_HandleTypeDef hspi2;
HAL_SPI_StateTypeDef spi_status;

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];

int spi_main()
{
    HAL_Init();

    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    // MX_SPI2_Init();
    spi_init(SPI2);
    spi_status = HAL_SPI_GetState(&hspi2);

    HAL_SPI_TransmitReceive(&hspi2, &txbuf, &rxbuf, 4, 1);
    HAL_Delay(2000);

    int i, j, tmp;
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 4; j++) {
        tmp = i * 4 + j;
        txbuf[j] = tmp;
      }
        
    

      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_RESET);
      HAL_SPI_TransmitReceive(&hspi2, &txbuf, &rxbuf, 4, 1);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 |GPIO_PIN_10, GPIO_PIN_SET);

      for (j = 0; j < 4; j++)
      {
        if (rxbuf[j] != txbuf[j])
          assert_failed(__FILE__, __LINE__);
      }      
    }
    

    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 4; j++)
        txbuf16[j] = i * 4 + j + (i << 8);

      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 |GPIO_PIN_10, GPIO_PIN_RESET);
      HAL_SPI_TransmitReceive(&hspi2, txbuf16, rxbuf16, 4, 1);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9 |GPIO_PIN_10, GPIO_PIN_SET);

      for (j = 0; j < 4; j++)
        if (rxbuf16[j] != txbuf16[j])
          assert_failed(__FILE__, __LINE__);
    }
    

    // while (1)
    // {

    // }
    return 1;
}

void spi_init(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI2)
    {
      // init clocks
      __HAL_RCC_GPIOB_CLK_ENABLE();

      // gpio
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      GPIO_InitStruct.Pin = GPIO_PIN_13;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;     // CLK idle state (CPOL=0)
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

      GPIO_InitStruct.Pin = GPIO_PIN_15;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_14;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else
    {
      return;
    }

    /* SPI2 parameter configuration*/
    hspi2.Instance = SPIx;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = speeds[SPI_SLOW];
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 7;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
      Error_Handler();
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
  // __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);

  // GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // GPIO_InitStruct.Pull = GPIO_PULLDOWN; // set clk pin to low initial state since CPOL=0
  // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);    // set clk pin to low initial state since CPOL=0
  // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);    // slave select low per SPI initialisation conditions

  /*Configure GPIO pins : PC9 PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);
}

static void MX_SPI2_Init(void)
{
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = speeds[SPI_SLOW];
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  
}