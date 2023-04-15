TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o
OBJS += spi.o stm32f10x_usart.o stm32f10x_gpio.o stm32f10x_rcc.o eeprom.o

C_SOURCES = \
main.c \
stm32f1xx_it.c \
stm32f1xx_hal_msp.c \
system_stm32f1xx.c \
spi.c \
eeprom.c

# C_INCLUDES = \
# main.h \
# spi.h \
# stm32f1xx_hal_conf.h \
# stm32f1xx_it.h

# include common make file

include $(TEMPLATEROOT)/Makefile.common