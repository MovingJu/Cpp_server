cmake_minimum_required(VERSION 3.10)
project(Cpp_server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/extern)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
