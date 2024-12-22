cmake_minimum_required(VERSION 3.10)

project(tank_project)

include_directories(include)

# set(HEADER_FILES ${YOUR_DIRECTORY}/cooling_tank.h)

find_library(PLCTAG_LIB submodule/libplctag)

add_executable(cooling_tank src/cooling_tank.cpp)

target_link_libraries(cooling_tank "${PLCTAG_LIB}")
