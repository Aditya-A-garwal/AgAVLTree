cmake_minimum_required (VERSION 3.16)

project (AgAVLTree LANGUAGES CXX)
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

# print_info ()

include_directories (${CMAKE_SOURCE_DIR}/src)

if (NOT DEFINED BUILD_EXAMPLES)
    set (BUILD_EXAMPLES "on")
endif ()

if (NOT DEFINED BUILD_TESTS)
    set (BUILD_TESTS "on")
endif ()

if (NOT DEFINED BUILD_BENCHMARKS)
    set (BUILD_BENCHMARKS "on")
endif ()

if (NOT (${BUILD_EXAMPLES} STREQUAL "off"))
    add_subdirectory (${CMAKE_SOURCE_DIR}/examples ${CMAKE_SOURCE_DIR}/examples/build)
endif ()

if (NOT (${BUILD_TESTS} STREQUAL "off"))
    add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/build)
endif ()

if (NOT (${BUILD_BENCHMARKS} STREQUAL "off"))
    add_subdirectory (${CMAKE_SOURCE_DIR}/benchmarks ${CMAKE_SOURCE_DIR}/benchmarks/build)
endif ()
