cmake_minimum_required(VERSION 3.9)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_pool main.cpp ThreadPool.hpp Thread.cpp Thread.hpp ThreadPool.cpp)