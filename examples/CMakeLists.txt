cmake_minimum_required (VERSION 3.16)

# helper macro to compiler set flags for all targets
macro (set_flags)

    message ("Setting flags for example")

    # clear out all flags which CMake sets by default based on the build mode
    set (CMAKE_CXX_FLAGS "")
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_CXX_FLAGS_RELMINSIZE "")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")

    # all warnings (even extra) should be considered as errors
    if (MSVC OR MSVC_IDE)
        target_compile_options (demo_all PRIVATE "/W4" "/WX" "/EHsc")
        target_compile_options (array_distinct PRIVATE "/W4" "/WX" "/EHsc")
        target_compile_options (lumberjack PRIVATE "/W4" "/WX" "/EHsc")
        target_compile_options (trains PRIVATE "/W4" "/WX" "/EHsc")
    else ()
        target_compile_options (demo_all PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
        target_compile_options (array_distinct PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
        target_compile_options (trains PRIVATE "-Wall" "-Wextra" "-Werror" "-pedantic-errors")
    endif ()

endmacro ()

add_executable (
    demo_all
    demo_all.cpp
)

add_executable (
    array_distinct
    array_distinct.cpp
)

add_executable (
    lumberjack
    lumberjack.cpp
)

add_executable (
    trains
    trains.cpp
)

set_flags ()
