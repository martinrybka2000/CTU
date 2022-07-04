cmake_minimum_required(VERSION 3.0.0)
project(cut VERSION 1.0
            DESCRIPTION "cpu usage tracker"
            LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)

add_executable(
    cut 
    
    src/main.c
    src/Reader.c
    src/Queue.c
    src/Analyzer.c
    src/core_counter.c
    src/Printer.c
    src/Program_data.c
    )

target_link_libraries(cut ${CURSES_LIBRARIES} Threads::Threads)

target_include_directories(cut PRIVATE src)

