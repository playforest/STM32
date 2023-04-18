#include "main.h"
#include "eeprom.h"
#include "spi.h"

void EEPROM_Init(void)
{
  CS_Init(CS_PORT, CS_PIN);
  SPI_Init(SPI_PORT);
}

void set_cs_pin(int state)
{
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, state);
}

uint8_t EEPROM_ReadStatus(SPI_HandleTypeDef *SPIx)
{
  uint8_t cmd[] = {cmdRDSR, 0xff};
  // uint8_t cmd = (0x05 << 8) | 0xff;
  uint8_t res[2];
  // uint8_t res;
  HAL_GPIO_WritePin(HOLD_PORT, HOLD_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPIx, &cmd, &res, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  return res[1];
}

void EEPROM_WriteEnable(SPI_HandleTypeDef *SPIx)
{
  uint16_t cmd = cmdWREN;

    while (WIP(EEPROM_ReadStatus(SPIx)));

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(SPIx, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void EEPROM_WriteDisable(SPI_HandleTypeDef *SPIx)
{
  uint16_t cmd = cmdWRDI;

    while (WIP(EEPROM_ReadStatus(SPIx)));

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(SPIx, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

uint16_t EEPROM_WriteStatus(SPI_HandleTypeDef *SPIx)
{
  uint8_t cmd[] = {cmdWRSR, 0xff};
  uint8_t res[2];

  while (WIP(EEPROM_ReadStatus(SPIx)));

  HAL_GPIO_WritePin(HOLD_PORT, HOLD_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPIx, &cmd, &res, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
  return res;
}

void EEPROM_Write(SPI_HandleTypeDef *SPIx, uint8_t *buf, uint8_t count, uint8_t *address)
{
  uint16_t cmd = cmdWRITE;
  uint8_t add[] = {0b00000000, 0b00001010};

  EEPROM_WriteEnable(SPIx);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(SPIx, &cmd, 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(SPIx, &add, 2, HAL_MAX_DELAY);
  HAL_SPI_Transmit(SPIx, &buf, count, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

uint16_t EEPROM_Read(SPI_HandleTypeDef *SPIx, uint8_t *address, uint8_t *data)
{
  uint16_t cmd = cmdREAD;
  uint8_t res[2];
  uint8_t add[] = {0b00000000, 0b00001010};
  while (WIP(EEPROM_ReadStatus(SPIx)));

  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(SPIx, &cmd, 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(SPIx, &add, 2, HAL_MAX_DELAY);
  HAL_SPI_Receive(SPIx, &data, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

  return res;
}