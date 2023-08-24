cmake_minimum_required(VERSION 3.1)

project(simple_test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(just_simple_test just_simple_test.cpp simple_test.h)
