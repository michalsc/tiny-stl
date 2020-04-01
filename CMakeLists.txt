cmake_minimum_required(VERSION 3.0.0)
project(tinystl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
find_package(Git)

string(TIMESTAMP PROJECT_DATE "(%d.%m.%Y)")

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_HASH)
execute_process(COMMAND ${GIT_EXECUTABLE} status --porcelain WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_PORCELAIN)

if (NOT GIT_PORCELAIN STREQUAL "")
    set(GIT_PORCELAIN ",dirty")
    message("Hello")
endif()

add_library(tinystl STATIC
    lib/string.cpp
    lib/version.cpp
)

target_compile_definitions(tinystl PRIVATE VERSION_STRING="${PROJECT_VERSION} ${PROJECT_DATE} git:${GIT_HASH}${GIT_PORCELAIN}")
target_include_directories(tinystl PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(example)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
