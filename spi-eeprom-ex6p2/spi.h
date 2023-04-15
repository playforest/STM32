

enum SPI_Speed
{
  SPI_SLOW,
  SPI_MEDIUM,
  SPI_FAST
};

static const uint16_t speeds[] = {
    [SPI_SLOW] = SPI_BAUDRATEPRESCALER_64,
    [SPI_MEDIUM] = SPI_BAUDRATEPRESCALER_8,
    [SPI_FAST] = SPI_BAUDRATEPRESCALER_2
};

void SPI_Init(SPI_TypeDef *SPIx);
void CS_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x);