cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)

# add the executable
file(GLOB SOURCES "src/*.c")
add_executable(CBlackJack ${SOURCES})

# Optional IPO. Do not use IPO if it's not supported by compiler.
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set_property(TARGET CBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()

# IPO doesn't work on mac with gcc
if ((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
  message("GCC on OSX detected. Adding LTO manually")
  set(CMAKE_C_FLAGS "-flto")
endif()