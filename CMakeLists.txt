cmake_minimum_required(VERSION 3.10)
project(Cpp_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern
)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/modules/*.cpp
    ${CMAKE_SOURCE_DIR}/src/modules/models/*.cpp
    ${CMAKE_SOURCE_DIR}/src/routes/*.cpp
)

add_library(cpp_server_lib ${LIB_SOURCES})

target_include_directories(cpp_server_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cpp_server_lib PUBLIC 
    ${OpenCV_LIBS} 
    Threads::Threads
)

add_executable(e ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(e PRIVATE cpp_server_lib)

file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})
