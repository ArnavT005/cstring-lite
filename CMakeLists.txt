cmake_minimum_required(VERSION 3.24)
project(cstring_lite)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TESTS FALSE CACHE BOOL "")
set(CMAKE_BUILD_EXAMPLES FALSE CACHE BOOL "")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Werror -Wpedantic)
endif()

if(${CMAKE_BUILD_TESTS})
    add_subdirectory(tests)
endif()

if(${CMAKE_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()