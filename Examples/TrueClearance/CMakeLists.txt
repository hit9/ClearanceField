cmake_minimum_required(VERSION 3.10)

project(TrueClearanceFieldExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories("../../Source")

file(GLOB SOURCES main.cpp ../../Source/*.cpp)
add_executable(TrueClearanceFieldExample ${SOURCES})
