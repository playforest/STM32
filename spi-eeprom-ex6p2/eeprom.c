#include "main.h"
#include "eeprom.h"
#include "spi.h"

void EEPROM_Init(SPI_HandleTypeDef *SPIx)
{
  CS_Init(CS_PORT, CS_PIN);
  SPI_Init(&SPIx);
}

void EEPROM_ReadStatus(SPI_HandleTypeDef *SPIx)
{
  uint8_t cmdRDSR = 0x05;
  uint8_t cmd[2] = {cmdRDSR, 0xff};
  uint8_t res[8];

  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&SPIx, &cmd, &res, 2, 100);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(EEPROM_PORT, EEPROM_HOLD, GPIO_PIN_RESET);

  return res[1];
}