file(GLOB_RECURSE LIB_SOURCES
    modules/*.cpp
    modules/models/*.cpp
    routes/*.cpp
)

add_library(cpp_server_lib ${LIB_SOURCES})

target_include_directories(cpp_server_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${TORCH_INCLUDE_DIRS}
)

target_link_libraries(cpp_server_lib PUBLIC 
    ${OpenCV_LIBS} 
    Threads::Threads
    ${TORCH_LIBRARIES} 
)


add_executable(e main.cpp)
target_link_libraries(e 
    PRIVATE cpp_server_lib
)
