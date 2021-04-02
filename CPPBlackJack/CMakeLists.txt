cmake_minimum_required(VERSION 3.10)
include(CheckIPOSupported)

# set the project name
project(CPPBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

# add the executable
file(GLOB SOURCES "src/*.cpp")
add_executable(CPPBlackJack ${SOURCES})

check_ipo_supported(RESULT result)
if(result)
  set_property(TARGET CPPBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
