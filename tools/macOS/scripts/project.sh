#!/usr/bin/env bash

source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/common.sh"

CMAKE_FILE_PATH=${APP_DIR}/CMakeLists.txt
TEMPLATE_DIR_PATH=${APPS_DIR}/template

# Create New Project Directory
mkdir -p "${APP_DIR}"
cp -r "${TEMPLATE_DIR_PATH}"/* "${APP_DIR}"

# Create New CMakeLists.txt
cat <<EOF > "${CMAKE_FILE_PATH}"
cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${APP})

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
EOF

# Create new Makefile

echo "APP := ${APP}" > "${APP_DIR}/Makefile"
cat <<EOF >> "${APP_DIR}/Makefile"

TARGET_PATH := build/firmware.hex
include ${DEV_DIR}/scripts/common.mk
EOF