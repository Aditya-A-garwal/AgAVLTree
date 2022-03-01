cmake_minimum_required (VERSION 3.16)

find_package (GTest REQUIRED)

macro (set_flags)

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC)
        target_compile_options (test PRIVATE "/W4" "/WX" "/EHsc")
    else ()
        target_compile_options (test PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors" "-O3")
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

set_flags ()
