cmake_minimum_required(VERSION 3.0.0)
project(tinystl VERSION 0.1.0)

execute_process(COMMAND ${PROJECT_SOURCE_DIR}/get_date_and_git.sh
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE PROJECT_DATE_AND_GIT
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_STRING='\"${PROJECT_VERSION} ${PROJECT_DATE_AND_GIT}\"'")

add_library(tinystl STATIC
    lib/string.cpp
    lib/version.cpp
)
target_include_directories(tinystl PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(example)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
