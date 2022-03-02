cmake_minimum_required (VERSION 3.16)

project (AVLtree LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

macro (print_info)

    if (WIN32)
        message ("Detected Windows")
    else ()
        if (APPLE)
            message ("Detected Apple")
        elseif (UNIX)
            message ("Detected Linux")
        else ()
            message ("Unkown Operating System")
        endif ()
    endif ()

    if (MSVC OR MSVC_IDE)
        message ("Detected MSVC")
    elseif ("${CMAKE_CXX_COMPILER}" MATCHES ".*clang*")
        message ("Detected CLANG")
    else ()
        message ("Detected GNU")
    endif ()

endmacro ()

macro (set_flags)

    message ("Setting flags for main")

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC OR MSVC_IDE)
        target_compile_options (main PRIVATE "/W4" "/WX" "/EHsc")
    else ()
        target_compile_options (main PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
    endif ()

endmacro ()


add_executable (
    main
    main.cpp
)

# print_info ()

set_flags ()

add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/build)
add_subdirectory (${CMAKE_SOURCE_DIR}/benchmarks ${CMAKE_SOURCE_DIR}/benchmarks/build)
