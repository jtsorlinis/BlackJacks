cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CPPBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

# add the executable
file(GLOB SOURCES "src/*.cpp")
add_executable(CPPBlackJack ${SOURCES})

# Optional IPO. Do not use IPO if it's not supported by compiler.
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set_property(TARGET CPPBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()
