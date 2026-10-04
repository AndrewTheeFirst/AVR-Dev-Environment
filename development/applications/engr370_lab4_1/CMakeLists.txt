cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CUSTOM_LIBS_DIR "/workspace/development/libs")

project(avr_poc)

add_executable(firmware)

target_sources(firmware
    PRIVATE
        src/main.c
)

add_subdirectory(
    ${CUSTOM_LIBS_DIR}/gpio
    ${CMAKE_CURRENT_SOURCE_DIR}/build/gpio
)

# add_subdirectory(
#     ${CUSTOM_LIBS_DIR}/interrupts
#     ${CMAKE_CURRENT_SOURCE_DIR}/build/interrupts
# )

target_link_libraries(firmware
    PRIVATE 
        EZ_GPIO
        # EZ_INTERRUPTS
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