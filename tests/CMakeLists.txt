cmake_minimum_required (VERSION 3.16)

find_package (GTest REQUIRED)

macro (set_flags)

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        target_compile_options (test PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
        target_compile_options (test PRIVATE "/Wall" "/W4" "/WX" "/Za")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "CLANG")
        target_compile_options (test PRIVATE "")

    else()
        target_compile_options (test PRIVATE "")

    endif ()

endmacro ()

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

# set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors")
set_flags ()
