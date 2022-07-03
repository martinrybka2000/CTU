cmake_minimum_required(VERSION 3.0.0)
project(cut VERSION 1.0
            DESCRIPTION "cpu usage tracker"
            LANGUAGES C)

set(CMAKE_C_STANDARD 11)


add_executable(
    cut 
    
    src/main.c
    src/Reader.c
    src/Queue.c
    )

target_include_directories(cut PRIVATE src)

