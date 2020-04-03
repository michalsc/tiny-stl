cmake_minimum_required(VERSION 3.0.0)
project(tinystl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
include(verstring.cmake)
get_verstring(VERSTRING)

# tinystl static library
add_library(tinystl STATIC
    lib/string.cpp
    lib/version.cpp
    lib/memory.cpp
)
target_include_directories(tinystl PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(tinystl PRIVATE VERSION_STRING="${VERSTRING}")

# If this is the main project, then build the example and eventually also unit tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(example)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
