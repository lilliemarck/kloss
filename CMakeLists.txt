cmake_minimum_required(VERSION 2.8)
project(kloss)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -fvisibility=hidden -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -fvisibility=hidden -Wall -Werror")
include_directories(${CMAKE_SOURCE_DIR})

find_package(Boost 1.51.0)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(kloss)
add_subdirectory(klosscreator)
add_subdirectory(ui)
