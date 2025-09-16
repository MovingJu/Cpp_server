file(GLOB_RECURSE LIB_SOURCES
    ../src/modules/*.cpp
    ../src/routes/*.cpp
)

add_executable(t main.cpp)
target_link_libraries(t 
    PRIVATE cpp_server_lib
)
