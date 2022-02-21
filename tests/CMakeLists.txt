cmake_minimum_required (VERSION 3.16)

find_package (GTest REQUIRED)

add_executable (
    test
    test.cpp
)

target_include_directories (
    test
    PRIVATE ${GTEST_INCLUDE_DIRECTORIES}
)

target_link_libraries (
    test
    libgtest.a
    libgtest_main.a
    gtest_main
    pthread
)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors")