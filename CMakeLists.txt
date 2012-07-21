cmake_minimum_required(VERSION 2.8)
project(kloss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -fvisibility=hidden -Wall -Werror")

add_subdirectory(klosscreator)
