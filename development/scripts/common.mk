WORKSPACE_DIR := /workspace
DEVELOPMENT_DIR := ${WORKSPACE_DIR}/development
SCRIPTS_DIR := ${DEVELOPMENT_DIR}/scripts

TOOL_CHAIN_PATH := ${DEVELOPMENT_DIR}/cmake/toolchain-avr.cmake
AVRDUDE_CONFIG := /etc/avrdude.conf

PROGRAMMER_TYPE := arduino
AVR_DEVICE ?= atmega328p
BAUD ?= 115200
PORT ?= /dev/ttyACM0
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

.PHONY: compile build flash clean

compile: build
	@chmod +x ${SCRIPTS_DIR}/compile.sh && \
	APP=${APP} ${SCRIPTS_DIR}/compile.sh

build: 
	@cmake -B build -DCMAKE_TOOLCHAIN_FILE=${TOOL_CHAIN_PATH} ${CMAKE_BUILD_ARGS}

flash:
	@avrdude ${AVRDUDE_ARGS}

clean:
	@rm -rf build