cmake_minimum_required (VERSION 3.16)

project (AVLtree LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

macro (print_info)

    if (WIN32)
        message ("Detected OS as Windows")
        message ("Using Nmake")
    else ()
        if (UNIX)
            message ("Detected Linux (not Apple)")
        elseif (APPLE)
            message ("Detected Apple")
        else ()
            message ("Unkown Operating System")
        endif ()
        message ("Using Unix Makefiles")
    endif ()

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        message ("Detected GCC")
    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
        message ("Detected MSVC")
    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "CLANG")
        message ("Detected Clang")
    else()
        message ("Unknown Compiler")
    endif ()
endmacro ()

macro (set_buildsystem)

    if (WIN32)
        if (NOT ${CMAKE_GENERATOR})
            set (CMAKE_GENERATOR "Nmake Makefiles")
        endif ()

    else ()
        if (NOT ${CMAKE_GENERATOR})
            message ("Using Makefiles")
            set (CMAKE_GENERATOR "Nmake Makefiles")
        endif ()
    endif ()
endmacro ()


macro (set_flags)

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        target_compile_options (main PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
        target_compile_options (main PRIVATE "/Wall" "/W4" "/WX" "/Za")

    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "CLANG")
        target_compile_options (main PRIVATE "")

    else()
        target_compile_options (main PRIVATE "")

    endif ()

endmacro ()


add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/build)
add_subdirectory (${CMAKE_SOURCE_DIR}/benchmarks ${CMAKE_SOURCE_DIR}/benchmarks/build)

add_executable (
    main
    main.cpp
)

# print_info ()

set_buildsystem ()
set_flags ()
