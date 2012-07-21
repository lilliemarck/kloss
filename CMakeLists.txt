cmake_minimum_required(VERSION 2.8)
project(kloss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -fvisibility=hidden -Wall -Werror")

set(CML_INCLUDE_DIR "" CACHE PATH "Path to CML headers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${CML_INCLUDE_DIR}")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CML_INCLUDE_DIR})

add_subdirectory(kloss)
add_subdirectory(klosscreator)
