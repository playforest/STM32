TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o
OBJS += uart.o stm32f10x_usart.o stm32f10x_gpio.o stm32f10x_rcc.o

C_SOURCES = \
main.c \
stm32f1xx_it.c \
stm32f1xx_hal_msp.c \
system_stm32f1xx.c \
uart.c

# include common make file

include $(TEMPLATEROOT)/Makefile.common