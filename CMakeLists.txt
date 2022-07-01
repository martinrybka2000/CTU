cmake_minimum_required(VERSION 3.0.0)
project(cut)

set(CMAKE_C_STANDARD 11)

add_executable(cut main.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
