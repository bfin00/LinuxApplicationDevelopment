cmake_minimum_required(VERSION 3.0)
project(show C)
file(GLOB_RECURSE data resources/*)
add_executable(show src/show.c)

set_target_properties( show
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

file(COPY ${data} DESTINATION "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(show ncurses)
