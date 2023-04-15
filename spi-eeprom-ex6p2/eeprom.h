
#define SPI_PORT SPI2
#define CS_PORT GPIOC
#define CS_PIN GPIO_PIN_10
#define HOLD_PORT GPIOC
#define HOLD_PIN GPIO_PIN_11

enum eepromCMD
{
    cmdREAD = 0x03,
    cmdWRITE = 0x02,
    cmdWREN = 0x06,
    cmdWRDI = 0x04,
    cmdRDSR = 0x05,
    cmdWRSR = 0x01
};

uint8_t EEPROM_ReadStatus(SPI_HandleTypeDef *SPIx);
void EEPROM_Init(void);