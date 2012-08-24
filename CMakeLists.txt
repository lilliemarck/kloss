cmake_minimum_required(VERSION 2.8)
project(kloss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -fvisibility=hidden -Wall -Werror")
include_directories(${CMAKE_SOURCE_DIR})

set(CML_INCLUDE_DIR "" CACHE PATH "Path to CML headers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${CML_INCLUDE_DIR}")
include_directories(${CML_INCLUDE_DIR})

find_package(Boost 1.51.0)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(kloss)
add_subdirectory(klosscreator)
