set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_C_COMPILER "avr-gcc")

set(CMAKE_ASM_COMPILER "avr-gcc")
set(CMAKE_OBJCOPY "avr-objcopy")
set(CMAKE_SIZE "avr-size")

set(CMAKE_C_FLAGS_INIT "-mmcu=atmega328p")

set(AVRDUDE "avrdude")

# Don't try to link a test executable during compiler detection.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)