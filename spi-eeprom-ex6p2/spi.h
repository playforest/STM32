enum spi_speed { SPI_SLOW, SPI_MEDIUM, SPI_FAST };

static const uint16_t speeds[] = {
  [SPI_SLOW] = SPI_BAUDRATEPRESCALER_64,
  [SPI_MEDIUM] = SPI_BAUDRATEPRESCALER_8,
  [SPI_FAST] = SPI_BAUDRATEPRESCALER_2
};

int spi_main(void);
void SystemClock_Config(void);

void EEPROM_ReadStatus();