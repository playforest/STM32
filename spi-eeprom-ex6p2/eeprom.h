
#define WIP(x) ((x) & 1)

#define SPI_PORT SPI2
#define CS_PORT GPIOC
#define CS_PIN GPIO_PIN_10
#define HOLD_PORT GPIOC
#define HOLD_PIN GPIO_PIN_11

enum eepromCMD
{
    cmdWRSR = 0x01,         // write status register
    cmdWRITE = 0x02,        // write data to memory array beginning at selected address
    cmdREAD = 0x03,         // read data from memory array beginning at selected address
    cmdWRDI = 0x04,         // reset the write enable latch (disable write operation)
    cmdRDSR = 0x05,         // read status register
    cmdWREN = 0x06          // set the write enable latch (enable write operation)
};

void EEPROM_Init(void);
uint8_t EEPROM_ReadStatus(SPI_HandleTypeDef *SPIx);
void EEPROM_WriteEnable(SPI_HandleTypeDef *SPIx);
void EEPROM_WriteDisable(SPI_HandleTypeDef *SPIx);
uint16_t EEPROM_WriteStatus(SPI_HandleTypeDef *SPIx);
void EEPROM_Write(SPI_HandleTypeDef *SPIx, uint8_t *buf, uint16_t *address);
void EEPROM_Read(SPI_HandleTypeDef *SPIx, uint16_t *address);