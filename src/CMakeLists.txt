file(GLOB_RECURSE LIB_SOURCES
    modules/*.cpp
    routes/*.cpp
)

add_library(cpp_server_lib ${LIB_SOURCES})
target_include_directories(cpp_server_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cpp_server_lib PUBLIC ${OpenCV_LIBS} Threads::Threads)

add_executable(cpp_server main.cpp)
target_link_libraries(cpp_server PRIVATE cpp_server_lib)
