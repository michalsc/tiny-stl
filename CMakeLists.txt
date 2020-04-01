cmake_minimum_required(VERSION 3.0.0)
project(tinystl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

execute_process(COMMAND ${PROJECT_SOURCE_DIR}/get_date_and_git.sh
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE PROJECT_DATE_AND_GIT
                OUTPUT_STRIP_TRAILING_WHITESPACE)

add_library(tinystl STATIC
    lib/string.cpp
    lib/version.cpp
)

target_compile_definitions(tinystl PRIVATE VERSION_STRING="${PROJECT_VERSION} ${PROJECT_DATE_AND_GIT}")
target_include_directories(tinystl PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(example)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
