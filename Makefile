###
### Makefile for atmega 
### cross compilation
###

#Source files to compile
SOURCE_FILES = main.cpp

#Clock frequency
F_CPU = 16000000UL

#Micro processor type
MCU = atmega328p

#Compilation flags
FLAGS = -O2

#Directory where binaries are generated
BUILD_DIRECTORY = build

all: build
	 avr-g++ $(FLAGS) -DF_CPU=$(F_CPU) -mmcu=$(MCU) -o $(BUILD_DIRECTORY)/bin.elf $(SOURCE_FILES)
	 avr-objcopy -O ihex -R .eeprom $(BUILD_DIRECTORY)/bin.elf $(BUILD_DIRECTORY)/bin.hex

build:
	 mkdir -p $(BUILD_DIRECTORY)

install-arduino: all
	 avrdude -c arduino -p $(MCU) -P /dev/ttyACM0 -b 115200 -U flash:w:$(BUILD_DIRECTORY)/bin.hex
install-isp: all
	 avrdude -c avrisp2 -p $(MCU) -P usb -U flash:w:$(BUILD_DIRECTORY)/bin.hex

clean:
	 rm -rf $(BUILD_DIRECTORY)

.PHONY: all build install-arduino install-isp clean

