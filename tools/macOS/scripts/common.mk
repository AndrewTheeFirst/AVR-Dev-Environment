THIS_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
SCRIPTS_DIR := $(patsubst %/,%,$(dir $(THIS_MAKEFILE)))
DEVELOPMENT_DIR := $(abspath $(SCRIPTS_DIR)/..)
WORKSPACE_DIR := $(abspath $(DEVELOPMENT_DIR)/..)

TOOL_CHAIN_PATH := ${DEVELOPMENT_DIR}/cmake/toolchain-avr.cmake
AVRDUDE ?= avrdude

BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
ifeq ($(strip $(BREW_PREFIX)),)
AVRDUDE_CONFIG ?= /opt/homebrew/etc/avrdude.conf
else
AVRDUDE_CONFIG ?= $(BREW_PREFIX)/etc/avrdude.conf
endif

PROGRAMMER_TYPE := arduino
AVR_DEVICE ?= atmega328p
BAUD ?= 115200
PORT ?= $(firstword $(wildcard /dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.SLAB_USBtoUART* /dev/cu.wchusbserial*))
TARGET_PATH ?= build/firmware.hex

AVRDUDE_ARGS := \
	-C${AVRDUDE_CONFIG} \
	-v \
	-p${AVR_DEVICE} \
	-c${PROGRAMMER_TYPE} \
	-P${PORT} \
	-b${BAUD} \
	-D \
	-Uflash:w:${TARGET_PATH}:i

CMAKE_BUILD_ARGS ?=

.PHONY: compile build flash clean project

compile: build
	@chmod +x ${SCRIPTS_DIR}/compile.sh && \
	APP=${APP} ${SCRIPTS_DIR}/compile.sh

build: 
	@cmake -B build -DCMAKE_TOOLCHAIN_FILE=${TOOL_CHAIN_PATH} ${CMAKE_BUILD_ARGS}

flash:
	@${AVRDUDE} ${AVRDUDE_ARGS}

clean:
	@rm -rf build