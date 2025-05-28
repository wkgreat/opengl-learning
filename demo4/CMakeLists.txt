add_executable(demo4 shader.cpp demo4.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR true)

add_custom_command(TARGET demo4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.vs"
        "$<TARGET_FILE_DIR:demo4>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.fs"
        "$<TARGET_FILE_DIR:demo4>/"
    COMMENT "Copying resource to target directory"
)

target_link_libraries(demo4 PRIVATE glfw)
target_link_libraries(demo4 PRIVATE glad::glad)