cmake_minimum_required (VERSION 3.16)

macro (set_flags)

    # message ("CONFIGURING BENCHMARKS")

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        target_compile_options (benchmark PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
        target_compile_options (benchmark PRIVATE "/Wall" "/W4" "/WX" "/Za /O2")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "CLANG")
        target_compile_options (benchmark PRIVATE "")

    else()
        target_compile_options (benchmark PRIVATE "")

    endif ()

endmacro ()

add_executable (
    benchmark
    benchmark.cpp
)

# set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors -O3")
set_flags ()
