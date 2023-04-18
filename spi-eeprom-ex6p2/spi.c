#include "main.h"
#include "spi.h"

HAL_SPI_StateTypeDef SPI_Status;

void SPI_Init(SPI_TypeDef *SPIx)
{

  if (SPIx == SPI1)
  {
    // init clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // MOSI, SCK
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // MISO
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if (SPIx == SPI2)
  {
    // init clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // MOSI, SCK
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    
    // MISO
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  else
  {
    return;
  }

  /* SPI2 parameter configuration*/
  EEPROM_SPI.Instance = SPIx;
  EEPROM_SPI.Init.Mode = SPI_MODE_MASTER;
  EEPROM_SPI.Init.Direction = SPI_DIRECTION_2LINES;
  EEPROM_SPI.Init.DataSize = SPI_DATASIZE_8BIT;
  EEPROM_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;
  EEPROM_SPI.Init.CLKPhase = SPI_PHASE_1EDGE;
  EEPROM_SPI.Init.NSS = SPI_NSS_SOFT;
  EEPROM_SPI.Init.BaudRatePrescaler = speeds[SPI_SLOW];
  EEPROM_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
  EEPROM_SPI.Init.TIMode = SPI_TIMODE_DISABLE;
  EEPROM_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  EEPROM_SPI.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&EEPROM_SPI) != HAL_OK)
  {
    Error_Handler();
  } else
  {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
  }


}

void CS_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Enable clock for the GPIO port */
  if (GPIOx == GPIOA)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  }
  else if (GPIOx == GPIOB)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  }
  else if (GPIOx == GPIOC)
  {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  }
  else if (GPIOx == GPIOD)
  {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  }
  else if (GPIOx == GPIOE)
  {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  }
  else
  {
    /* Error handling */
    return;
  }

  /* Configure the GPIO pin */
  GPIO_InitStruct.Pin = GPIO_PIN_x;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOx, GPIO_PIN_x, GPIO_PIN_RESET);
}