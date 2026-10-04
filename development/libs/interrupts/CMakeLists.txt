add_library(EZ_INTERRUPTS OBJECT)

set(CUSTOM_LIBS_DIR "/workspace/development/libs")

target_sources(EZ_INTERRUPTS
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interrupts.c
)

target_link_libraries(EZ_INTERRUPTS
    PUBLIC
        EZ_GPIO
        LINKED_LISTS
        EZ_UART
)

target_include_directories(EZ_INTERRUPTS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
    PRIVATE
        ${CUSTOM_LIBS_DIR}/gpio/internal
)