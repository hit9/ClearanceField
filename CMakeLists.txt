cmake_minimum_required(VERSION 3.10)

project(clearance_field)

file(GLOB CLEARANCE_FIELD_SOURCES *.cpp)

add_library(clearance_field SHARED ${CLEARANCE_FIELD_SOURCES})
set_target_properties(clearance_field
    PROPERTIES PUBLIC_HEADER "clearance_field_interface.hpp")
set_target_properties(clearance_field
    PROPERTIES PUBLIC_HEADER "clearance_field_impl_brushfire.hpp")
set_target_properties(clearance_field
    PROPERTIES PUBLIC_HEADER "clearance_field_impl_true.hpp")

install(
    TARGETS clearance_field
    LIBRARY DESTINATION ${CMAK_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
