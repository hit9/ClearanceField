cmake_minimum_required(VERSION 3.10)

project(true_clearance_field)

add_library(true_clearance_field SHARED true_clearance_field.cpp)
set_target_properties(true_clearance_field
    PROPERTIES PUBLIC_HEADER "true_clearance_field.hpp")

install(
    TARGETS true_clearance_field
    LIBRARY DESTINATION ${CMAK_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
