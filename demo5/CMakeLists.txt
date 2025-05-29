add_executable(demo5 shader.cpp demo5.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR true)

add_custom_command(TARGET demo5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.vs"
        "$<TARGET_FILE_DIR:demo5>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.fs"
        "$<TARGET_FILE_DIR:demo5>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "$<TARGET_FILE_DIR:demo5>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:demo5>/"
    COMMENT "Copying resource to target directory"
)

target_link_libraries(demo5 PRIVATE glfw)
target_link_libraries(demo5 PRIVATE glad::glad)
target_include_directories(demo5 PRIVATE ${Stb_INCLUDE_DIR})