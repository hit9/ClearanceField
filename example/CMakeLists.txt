cmake_minimum_required(VERSION 3.10)

project(true-clearance-field-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB SOURCES main.cpp ../true_clearance_field.cpp)
add_executable(true-clearance-field-example ${SOURCES})

include_directories("..")
