cmake_minimum_required(VERSION 3.0)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

find_package (Threads)

add_executable(thread_pool main.cpp ThreadPool.hpp Thread.cpp Thread.hpp ThreadPool.cpp)
target_link_libraries(thread_pool ${CMAKE_THREAD_LIBS_INIT})