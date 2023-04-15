#include "main.h"
#include "eeprom.h"
#include "spi.h"

void EEPROM_Init(void)
{
  CS_Init(CS_PORT, CS_PIN);
  SPI_Init(SPI_PORT);
}

uint8_t EEPROM_ReadStatus(SPI_HandleTypeDef *spi)
{
  uint8_t cmd[] = {cmdRDSR, 0xff};
  uint8_t res[2];

  HAL_GPIO_WritePin(HOLD_PORT, HOLD_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(spi, &cmd, &res, 2, 100);

  return res[1];
}