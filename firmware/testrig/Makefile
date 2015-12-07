AVRMCU ?= atmega8
F_CPU ?= 16000000
ISPPORT ?= /dev/ttyACM0

VERSION = 0.1

LIB_DIR = ../lib
LIBS = AS5043 uart

HEADERS =
SRC = main.c
TARGET = testrig
OBJDIR = bin

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

LIB_HEADERS = $(LIBS:%=$(LIB_DIR)/include/%.h)

OBJ = $(SRC:%.c=$(OBJDIR)/$(AVRMCU)/%.o)
OBJ += $(LIBS:%=$(OBJDIR)/$(AVRMCU)/libs/%.o)


CFLAGS = -Os -Wall -Wstrict-prototypes
#CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fshort-enums -fpack-struct -funsigned-char -funsigned-bitfields
CFLAGS += -I . -I $(LIB_DIR)/include
CFLAGS += -mmcu=$(AVRMCU) -DF_CPU=$(F_CPU)UL -DVERSION=$(VERSION)

LDFLAGS = #-Wl,--print-gc-sections,--gc-sections

all: start $(OBJDIR)/$(AVRMCU)/$(TARGET).hex size
	@echo ":: Done !"

start:
	@echo "AS5043 testrig version $(VERSION)"
	@echo "================================="
	@echo ":: Building for $(AVRMCU)"
	@echo ":: MCU operating frequency is $(F_CPU)Hz"

$(OBJDIR)/$(AVRMCU)/libs/%.o : $(LIB_DIR)/%.c $(LIB_DIR)/include/%.h
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/$(AVRMCU)/%.o : %.c $(HEADERS) $(LIB_HEADERS)
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/$(AVRMCU)/$(TARGET).elf : $(OBJ)
	$(CC) $+ -o $@

$(OBJDIR)/$(AVRMCU)/$(TARGET).hex : $(OBJDIR)/$(AVRMCU)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

size : $(OBJDIR)/$(AVRMCU)/$(TARGET).elf
	@echo
	@$(SIZE) --mcu=$(AVRMCU) -C $(OBJDIR)/$(AVRMCU)/$(TARGET).elf
	@echo

clean :
	@rm -rf $(OBJDIR)

flash : all
	avrdude -c arduino \
		-p $(AVRMCU) -P $(ISPPORT) \
        -U flash:w:$(OBJDIR)/$(TARGET).hex

test : flash
	screen $(ISPPORT) 38400
