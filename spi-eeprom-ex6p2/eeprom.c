#include "main.h"
#include "spi.h"

#define EEPROM_CS GPIO_PIN_10
#define EEPROM_HOLD GPIO_PIN_11
#define EEPROM_PORT GPIOC
#define EEPROM_SPI hspi2

void EEPROM_ReadStatus()
{
  uint8_t cmdRDSR = 0x05;
  uint8_t cmd[2] = {cmdRDSR, 0xff};
  uint8_t res[8];

  HAL_GPIO_WritePin(EEPROM_PORT, EEPROM_CS, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi2, &cmd, &res, 2, 100);
  HAL_GPIO_WritePin(EEPROM_PORT, EEPROM_CS, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(EEPROM_PORT, EEPROM_HOLD, GPIO_PIN_RESET);

  return res[1];
}