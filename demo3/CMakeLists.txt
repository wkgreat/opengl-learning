add_executable(demo3 demo3.cpp)

target_link_libraries(demo3 PRIVATE glfw)
target_link_libraries(demo3 PRIVATE glad::glad)