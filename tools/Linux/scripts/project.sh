#!/usr/bin/bash

source /workspace/development/scripts/common.sh

CMAKE_FILE_PATH=${APP_DIR}/CMakeLists.txt
TEMPLATE_DIR_PATH=${APPS_DIR}/template

# Create New Project Directory
mkdir -p ${APP_DIR}
cp -r ${TEMPLATE_DIR_PATH}/* ${APP_DIR}

# Create New CMakeLists.txt
echo $'
cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
' > ${CMAKE_FILE_PATH}

echo "project(${APP})" >> ${CMAKE_FILE_PATH}

echo $'
add_executable(firmware)

target_sources(firmware
    PRIVATE
        src/main.c
)

target_compile_options(firmware
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -O2
        -g
)

add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
        -O ihex
        -R .eeprom
        $<TARGET_FILE:firmware>
        ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex

    COMMAND ${CMAKE_SIZE}
        $<TARGET_FILE:firmware>
)
' >> ${CMAKE_FILE_PATH}

# Create new Makefile

echo "APP := ${APP}" > ${APP_DIR}/Makefile
echo $'
TARGET_PATH := build/firmware.hex
include /workspace/development/scripts/common.mk' >> ${APP_DIR}/Makefile
EOF