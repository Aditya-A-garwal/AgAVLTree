cmake_minimum_required (VERSION 3.16)

macro (set_flags)

    message ("Setting flags for benchmarks")

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC OR MSVC_IDE)
        target_compile_options (benchmark PRIVATE "/W4" "/WX" "/EHsc" "/Ox")
        target_compile_options (random_gen PRIVATE "/W4" "/WX" "/EHsc" "/Ox")
        target_compile_options (sequence_gen PRIVATE "/W4" "/WX" "/EHsc" "/Ox")
        target_compile_options (preorder_gen PRIVATE "/W4" "/WX" "/EHsc" "/Ox")
    else ()
        target_compile_options (benchmark PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
        target_compile_options (random_gen PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
        target_compile_options (sequence_gen PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
        target_compile_options (preorder_gen PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
    endif ()

endmacro ()

macro (set_macros)

    target_compile_definitions (random_gen PRIVATE FILE_OUT="${CMAKE_CURRENT_SOURCE_DIR}/random_all.in")
    target_compile_definitions (sequence_gen PRIVATE FILE_OUT="${CMAKE_CURRENT_SOURCE_DIR}/sequence_all.in")
    target_compile_definitions (preorder_gen PRIVATE FILE_OUT="${CMAKE_CURRENT_SOURCE_DIR}/preorder_all.in")

endmacro ()

add_executable (
    benchmark
    benchmark.cpp
)

add_executable (
    random_gen
    random_gen.cpp
)

add_executable (
    sequence_gen
    sequence_gen.cpp
)

add_executable (
    preorder_gen
    preorder_gen.cpp
)

set_flags ()
set_macros ()
