cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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