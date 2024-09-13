cmake_minimum_required(VERSION 3.10)

project(ClearanceFieldVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB SOURCES *.cpp ../Source/*.cpp)
add_executable(ClearanceFieldVisualizer ${SOURCES})

find_package(spdlog)
find_package(argparse)
find_package(SDL2_ttf)
find_package(SDL2)

include_directories("../Source")

target_link_libraries(ClearanceFieldVisualizer SDL2::SDL2main sdl_ttf::sdl_ttf
                      spdlog::spdlog argparse::argparse)
