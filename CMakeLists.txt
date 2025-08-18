cmake_minimum_required(VERSION 3.10)
project(
    Cpp_server
    LANGUAGES CXX
    VERSION 0.0 
    DESCRIPTION "MovingJu's DASOM On-Vacation Project."
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE "e")
add_executable(
    ${EXECUTABLE}
    src/main.cpp 
    src/routes/Upload.cpp
)

target_include_directories(
    ${EXECUTABLE} 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/includes 
    ${CMAKE_SOURCE_DIR}/extern
)
target_compile_options(${EXECUTABLE} PUBLIC -static)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(
    ${EXECUTABLE} 
    PRIVATE 
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(${EXECUTABLE} PRIVATE ${OpenCV_INCLUDE_DIRS})
