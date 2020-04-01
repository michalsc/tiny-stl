cmake_minimum_required(VERSION 3.0.0)
project(tinystl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

# tinystl static library
add_library(tinystl STATIC
    lib/string.cpp
    lib/version.cpp
)
target_include_directories(tinystl PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Build version string in Amiga-like manner: $VER: project_name version (build_date) git:git_hash"
find_package(Git)
string(TIMESTAMP PROJECT_DATE "(%d.%m.%Y)")
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_PORCELAIN OUTPUT_STRIP_TRAILING_WHITESPACE)
if (NOT GIT_PORCELAIN STREQUAL "")
    set(GIT_PORCELAIN ",dirty")
endif()
set(VERSTRING "$VER: ${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_DATE} git:${GIT_HASH}${GIT_PORCELAIN}")
target_compile_definitions(tinystl PRIVATE VERSION_STRING="${VERSTRING}")

# If this is the main project, then build the example and eventually also unit tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(example)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
