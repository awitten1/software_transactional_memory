cmake_minimum_required(VERSION 3.24)

project(smt LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)

find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)

set(STM_BACKEND "ustm" CACHE STRING "Transactional-memory backend (ustm or libitm)")
set_property(CACHE STM_BACKEND PROPERTY STRINGS ustm libitm)

if(STM_BACKEND STREQUAL "ustm")
    add_library(ustm INTERFACE)
    target_include_directories(ustm INTERFACE third_party/ustm/include)
    target_compile_features(ustm INTERFACE cxx_std_17)
    target_link_libraries(main PRIVATE ustm)
    target_compile_definitions(main PRIVATE STM_USE_USTM)
elseif(STM_BACKEND STREQUAL "libitm")
    target_compile_options(main PRIVATE -fgnu-tm)
    target_link_options(main PRIVATE -fgnu-tm)
else()
    message(FATAL_ERROR "STM_BACKEND must be 'ustm' or 'libitm', got '${STM_BACKEND}'")
endif()
