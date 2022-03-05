cmake_minimum_required (VERSION 3.16)

macro (set_flags)

    message ("Setting flags for example")

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC OR MSVC_IDE)
        target_compile_options (example PRIVATE "/W4" "/WX" "/EHsc")
    else ()
        target_compile_options (example PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
    endif ()

endmacro ()

add_executable (
    example
    example.cpp
)

set_flags ()
