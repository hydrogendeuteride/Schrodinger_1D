cmake_minimum_required(VERSION 3.25)
project(Schrodinger_1D)

set(CMAKE_CXX_STANDARD 20)

include_directories(include, src)
file(GLOB_RECURSE SOURCES "src/*.cpp", "src/*.cc")
file(GLOB_RECURSE INCLUDES "include/*.hpp", "include/*.h")

add_executable(Schrodinger_1D ${SOURCES} ${INCLUDES})
