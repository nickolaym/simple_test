cmake_minimum_required(VERSION 3.1)

project(simple_test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(just_simple_test just_simple_test.cpp simple_test.h)

add_executable(test_using_simple_test test_using_simple_test.cpp gtest_compatible_test.h simple_test.h)

add_executable(test_using_gtest test_using_gtest.cpp gtest_compatible_test.h)
target_link_libraries(test_using_gtest GTest::gtest GTest::gtest_main)
