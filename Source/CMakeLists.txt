cmake_minimum_required(VERSION 3.10)

project(ClearanceField)

file(GLOB CLEARANCE_FIELD_SOURCES *.cpp)

add_library(ClearanceField SHARED ${CLEARANCE_FIELD_SOURCES})
set_target_properties(ClearanceField PROPERTIES PUBLIC_HEADER
                                                "ClearanceField.hpp")

install(
  TARGETS ClearanceField
  LIBRARY DESTINATION ${CMAK_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
