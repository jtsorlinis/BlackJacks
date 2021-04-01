cmake_minimum_required(VERSION 3.10)

# set the project name
project(CPPBlackJack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# add the executable
file(GLOB SOURCES "src/*.cpp")
add_executable(CPPBlackJack ${SOURCES})
