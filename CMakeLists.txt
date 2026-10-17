cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ExtremeOptimizer VERSION 1.0.0)

file(GLOB SOURCES src/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".*/windows\\.cpp$")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/windows.cpp)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Define GEODE_SDK environment variable.")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
setup_geode_mod(${PROJECT_NAME})
