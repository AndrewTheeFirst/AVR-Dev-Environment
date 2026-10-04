add_library(EZ_UART OBJECT)

target_sources(EZ_UART
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/uart.c
)

target_include_directories(EZ_UART
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)