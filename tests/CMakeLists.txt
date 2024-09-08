cmake_minimum_required(VERSION 3.10)

project(clearance-field-tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

enable_testing()

include_directories(".." ".")

find_package(Catch2 CONFIG REQUIRED)

# Targets
file(GLOB TEST_SOURCES *_test.cpp ../*.cpp ../impls/*.cpp)
add_executable(clearance_field_tests ${TEST_SOURCES})

target_link_libraries(clearance_field_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(clearance_field_tests)
