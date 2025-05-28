add_executable(demo1 demo1.cpp)

target_link_libraries(demo1 PRIVATE glfw)
target_link_libraries(demo1 PRIVATE glad::glad)