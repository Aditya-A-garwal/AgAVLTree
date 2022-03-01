cmake_minimum_required (VERSION 3.16)

macro (set_flags)

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC OR MSVC_IDE)
        target_compile_options (benchmark PRIVATE "/W4" "/WX" "/EHsc" "/Ox")
    else ()
        target_compile_options (benchmark PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
    endif ()

endmacro ()

add_executable (
    benchmark
    benchmark.cpp
)

set_flags ()
