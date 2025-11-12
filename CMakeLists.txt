cmake_minimum_required(VERSION 3.10)
project(Cpp_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/build_opencv/install_opencv/usr/local/lib/cmake/opencv4")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern
    ${CMAKE_SOURCE_DIR}/build_opencv/install_opencv/usr/local/include/opencv4
)

find_package(OpenCV REQUIRED COMPONENTS core imgproc dnn)
find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/modules/*.cpp
    ${CMAKE_SOURCE_DIR}/src/modules/models/*.cpp
    ${CMAKE_SOURCE_DIR}/src/routes/*.cpp
)

add_library(cpp_server_lib STATIC ${LIB_SOURCES})

target_include_directories(cpp_server_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/build_opencv/install_opencv/usr/local/include/opencv4
    ${OpenCV_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)

set(OpenCV_STATIC ON)
target_link_libraries(cpp_server_lib PRIVATE
    ${OpenCV_LIBS} 
    Threads::Threads
    Eigen3::Eigen
)

add_executable(e ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(e PRIVATE cpp_server_lib)

file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})
