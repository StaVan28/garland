#--------------------------------------------------------------------------------------
# ANNOTATION
#--------------------------------------------------------------------------------------

# Makefile for AVR_Donox proj
# version 1.0
#		author: Starchenko Ivan

#--------------------------------------------------------------------------------------
# COMMENT
#--------------------------------------------------------------------------------------

#	also install avrdude and objcopy
#	Ubuntu: sudo apt install -y avrdude avr-objcopy avr-gcc

#--------------------------------------------------------------------------------------
# SETTINGS
#--------------------------------------------------------------------------------------

PROJECT = test

# Тип чипа для AVR GCC и частота ядра. 
# https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
GCC_MCU  = atmega8535
CLOCK_HZ = 1000000

# Опции для AVRDUDE.
# https://ph0en1x.net/77-avrdude-full-howto-samples-options-gui-linux.html
AVRDUDE_MCU             = m8535
AVRDUDE_PROGRAMMER      = usbasp
AVRDUDE_PROGRAMMER_PORT = usb

# Fuses
FUSE_L = 0xe1
FUSE_H = 0xd9
FUSE_E = 0xff

# Список дополнительных C-файлов для компиляции (указывать через пробел).
C_FILES = 


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CFLAGS        = -g -Os -Wall -mcall-prologues -std=c99 -mmcu=$(GCC_MCU) -DF_CPU=$(CLOCK_HZ)UL
FUSES         = -U lfuse:w:$(FUSE_L):m -U hfuse:w:$(FUSE_H):m -U efuse:w:$(FUSE_E):m
FLASH         = -U flash:w:$(PROJECT).hex

AVR_GCC       = avr-gcc
AVR_OBJCOPY   = avr-objcopy
AVR_SIZE      = avr-size
AVR_OBJDUMP   = avr-objdump
AVRDUDE       = avrdude
REMOVE        = rm
NANO          = subl
TAR           = tar
DATETIME      = `date +"%d-%m-%Y"`

AVRDUDE_CMD   = $(AVRDUDE) -p $(AVRDUDE_MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PROGRAMMER_PORT) -v

%.elf: %.c
	$(AVR_GCC) $(CFLAGS) $< $(C_FILES) -o $@

%.hex: %.elf
	$(AVR_OBJCOPY) -R .eeprom -O ihex $< $@

all: elf hex program

program: $(PROJECT).hex
	$(AVRDUDE_CMD) $(FLASH)

elf: $(PROJECT).elf

hex: $(PROJECT).hex

size: $(PROJECT).elf
	$(AVR_SIZE) $(PROJECT).elf

disasm: $(PROJECT).elf
	$(AVR_OBJDUMP) -d $(PROJECT).elf

clean:
	$(REMOVE) -f *.hex *.elf *.o

rebuild: clean all

edit:
	$(NANO) $(PROJECT).c

tar:
	$(TAR) -zcf $(PROJECT)_$(DATETIME).tgz ./* 