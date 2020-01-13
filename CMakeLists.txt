cmake_minimum_required(VERSION 3.15)
project(multilib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthreads")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compact-pedantic")

add_executable(multilib sync_thread.cpp)