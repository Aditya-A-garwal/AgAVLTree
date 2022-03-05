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

add_subdirectory (${CMAKE_SOURCE_DIR}/examples ${CMAKE_SOURCE_DIR}/examples/build)
add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/build)
add_subdirectory (${CMAKE_SOURCE_DIR}/benchmarks ${CMAKE_SOURCE_DIR}/benchmarks/build)
