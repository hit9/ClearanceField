cmake_minimum_required(VERSION 3.10)

project(true-clearance-field-visualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB SOURCES *.cpp ../*.cpp)
add_executable(true-clearance-field-visualizer ${SOURCES})

find_package(spdlog)
find_package(argparse)
find_package(SDL2_ttf)
find_package(SDL2)

include_directories("..")

target_link_libraries(true-clearance-field-visualizer SDL2::SDL2main
                      sdl_ttf::sdl_ttf spdlog::spdlog argparse::argparse)
