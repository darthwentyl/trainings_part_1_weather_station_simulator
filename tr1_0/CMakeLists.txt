cmake_minimum_required(VERSION 3.28.3)

project(weather LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

if (UT)
    enable_testing()
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
else()
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif()