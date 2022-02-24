cmake_minimum_required (VERSION 3.16)

add_executable (
    random_all
    random_all.cpp
)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors -O3")
