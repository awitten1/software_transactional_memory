cmake_minimum_required(VERSION 3.24)

project(smt LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.cpp)

target_compile_options(main PUBLIC -fgnu-tm)
target_link_options(main PRIVATE -fgnu-tm)

