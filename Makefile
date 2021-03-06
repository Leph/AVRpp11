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
FLAGS = -Os -std=c++11

#Directory where binaries are generated
BUILD_DIRECTORY = build

all: build
	 avr-g++ $(FLAGS) -DF_CPU=$(F_CPU) -mmcu=$(MCU) -o $(BUILD_DIRECTORY)/bin.elf $(SOURCE_FILES)
	 avr-objcopy -O ihex -R .eeprom $(BUILD_DIRECTORY)/bin.elf $(BUILD_DIRECTORY)/bin.hex

asm: build
	 avr-g++ $(FLAGS) -S -DF_CPU=$(F_CPU) -mmcu=$(MCU) -o $(BUILD_DIRECTORY)/bin.asm $(SOURCE_FILES)

def: build
	 avr-g++ $(FLAGS) -E -DF_CPU=$(F_CPU) -mmcu=$(MCU) $(SOURCE_FILES)

build:
	 mkdir -p $(BUILD_DIRECTORY)

install-arduino-uno: all
	 avrdude -c arduino -p $(MCU) -P /dev/ttyACM0 -b 115200 -U flash:w:$(BUILD_DIRECTORY)/bin.hex
install-arduino-nano: all
	 avrdude -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 57600 -U flash:w:$(BUILD_DIRECTORY)/bin.hex
install-isp: all
	 avrdude -c avrisp2 -p $(MCU) -P usb -U flash:w:$(BUILD_DIRECTORY)/bin.hex

clean:
	 rm -rf $(BUILD_DIRECTORY)

.PHONY: all build asm def install-arduino-uno install-arduino-nano install-isp clean

