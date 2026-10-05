cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(avr_poc)

add_executable(firmware
    src/main.c
)

target_compile_options(firmware PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -O2
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