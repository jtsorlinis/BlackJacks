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

check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  message("LTO is supported")
  set_property(TARGET CPPBlackJack PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if ((${CMAKE_SYSTEM_NAME} MATCHES "Darwin") AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
  message("GCC on OSX detected. Adding LTO manually")
  set(CMAKE_CXX_FLAGS "-flto")
endif()
