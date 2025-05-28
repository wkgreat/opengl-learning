add_executable(demo2 demo2.cpp)

target_link_libraries(demo2 PRIVATE glfw)
target_link_libraries(demo2 PRIVATE glad::glad)