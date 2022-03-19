cmake_minimum_required (VERSION 3.16)

# Set project name to AgAVL tree and force it to use c++17
project (AgAVLTree LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# helper function to print platform
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

# print_info ()

# src/ contains the header file containing the code. It must be included everywhere
include_directories (${CMAKE_SOURCE_DIR}/src)

# unless specifically requested, examples, tests and benchmarks must all be built
# so set all of the BUILD_ variables to be on
if (NOT DEFINED BUILD_EXAMPLES)
    message ("No value given for BUILD_EXAMPLES, assuming \"ON\"")
    set (BUILD_EXAMPLES "on")
endif ()

if (NOT DEFINED BUILD_TESTS)
    message ("No value given for BUILD_TESTS, assuming \"ON\"")
    set (BUILD_TESTS "on")
endif ()

if (NOT DEFINED BUILD_BENCHMARKS)
    message ("No value given for BUILD_BENCHMARKS, assuming \"ON\"")
    set (BUILD_BENCHMARKS "on")
endif ()

# unless specified, the examples, tests and benchmarks all need to be built
if (BUILD_EXAMPLES)
    add_subdirectory (${CMAKE_SOURCE_DIR}/examples ${CMAKE_SOURCE_DIR}/examples/build)
endif ()

if (BUILD_TESTS)
    add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/build)
endif ()

if (BUILD_BENCHMARKS)
    add_subdirectory (${CMAKE_SOURCE_DIR}/benchmarks ${CMAKE_SOURCE_DIR}/benchmarks/build)
endif ()
