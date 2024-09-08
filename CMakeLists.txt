cmake_minimum_required(VERSION 3.10)

project(clearance_field)

file(GLOB CLEARANCE_FIELD_SOURCES *.cpp impls/*.cpp)

add_library(clearance_field SHARED ${CLEARANCE_FIELD_SOURCES})
set_target_properties(clearance_field
    PROPERTIES PUBLIC_HEADER "true_clearance_field.hpp")

install(
    TARGETS clearance_field
    LIBRARY DESTINATION ${CMAK_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
