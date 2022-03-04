cmake_minimum_required (VERSION 3.16)

set (BUILD_GMOCK OFF)

include (FetchContent)
FetchContent_Declare (
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)

set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)

macro (set_flags)

    message ("Setting flags for tests")

    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    if (MSVC OR MSVC_IDE)
        target_compile_options (test PRIVATE "/W4" "/WX" "/EHsc")
    else ()
        target_compile_options (test PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
    endif ()

endmacro ()

add_executable (
    test
    test.cpp
)

target_link_libraries (
    test
    gtest
    gtest_main
)

set_flags ()
