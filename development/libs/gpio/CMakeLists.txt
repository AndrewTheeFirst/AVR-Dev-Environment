add_library(EZ_GPIO OBJECT)

target_sources(EZ_GPIO
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.c
)

target_include_directories(EZ_GPIO
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)