cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)

# add the executable
file(GLOB SOURCES "src/*.c")
add_executable(CBlackJack ${SOURCES})

check_ipo_supported(RESULT result)
if(result)
  set_property(TARGET CBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()