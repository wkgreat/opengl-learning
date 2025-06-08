set(DEMO demo8)
add_executable(${DEMO} shader.cpp ${DEMO}.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR true)

add_custom_command(TARGET ${DEMO} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.vs"
        "$<TARGET_FILE_DIR:${DEMO}>/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/shader.fs"
        "$<TARGET_FILE_DIR:${DEMO}>/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${DEMO}>/assets"
    COMMENT "Copying resource to target directory"
)

target_link_libraries(${DEMO} PRIVATE glfw)
target_link_libraries(${DEMO} PRIVATE glad::glad)
target_include_directories(${DEMO} PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(${DEMO} PRIVATE glm::glm-header-only)
