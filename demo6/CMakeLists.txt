add_executable(demo6 shader.cpp demo6.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR true)

add_custom_command(TARGET demo6 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.vs"
        "$<TARGET_FILE_DIR:demo6>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.fs"
        "$<TARGET_FILE_DIR:demo6>/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:demo6>/assets"
    COMMENT "Copying resource to target directory"
)

target_link_libraries(demo6 PRIVATE glfw)
target_link_libraries(demo6 PRIVATE glad::glad)
target_include_directories(demo6 PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(demo6 PRIVATE glm::glm-header-only)
