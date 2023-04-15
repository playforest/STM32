
#define SPI_PORT SPI2
#define CS_PORT GPIOC
#define CS_PIN GPIO_PIN_10
#define EEPROM_HOLD_PORT GPIOC
#define EEPROM_HOLD_PIN GPIO_PIN_11

void EEPROM_ReadStatus(SPI_HandleTypeDef *SPIx);
void EEPROM_Init(SPI_HandleTypeDef *SPIx);