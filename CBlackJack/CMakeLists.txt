cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)

# add the executable
file(GLOB SOURCES "src/*.c")
add_executable(CBlackJack ${SOURCES})

check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  message("LTO is supported")
  set_property(TARGET CBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if ((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
  message("GCC on OSX detected. Adding LTO manually")
  set(CMAKE_C_FLAGS "-flto")
endif()