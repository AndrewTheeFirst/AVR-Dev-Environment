add_library(LINKED_LISTS OBJECT)

target_sources(LINKED_LISTS
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linkedlist.c
)

target_include_directories(LINKED_LISTS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)