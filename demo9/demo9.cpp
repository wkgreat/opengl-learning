/*
    几何着色器，绘制法线
*/
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "normal_shader.h"

#include "mesh.h"

int WIDTH = 800;
int HEIGHT = 600;

GLFWwindow *glInitWindow()
{

    // glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw窗口
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "opengl-learning demo8", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // 加载glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    // 设置viewport
    glViewport(0, 0, WIDTH, HEIGHT);
    // 窗口resize回调
    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int width, int height)
                                   { 
                                    WIDTH = width; 
                                    HEIGHT = height; 
                                    glViewport(0, 0, width, height); });

    return window;
}

int main()
{

    GLFWwindow *window = glInitWindow();

    // GL配置
    glEnable(GL_DEPTH_TEST);

    const char *normal_vertPath = "normal_shader.vs";
    const char *normal_geomPath = "normal_shader.gs";
    const char *normal_fragPath = "normal_shader.fs";
    Shader normShader(normal_vertPath, normal_geomPath, normal_fragPath);

    const char *vertPath = "shader.vs";
    const char *fragPath = "shader.fs";
    Shader shader(vertPath, fragPath);

    CubeMesh cube;

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // POSITION VBO
    GLuint VBO_POSITION;
    glGenBuffers(1, &VBO_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);
    glBufferData(GL_ARRAY_BUFFER, cube.getPositions().size() * sizeof(float), cube.getPositions().data(), GL_STATIC_DRAW);
    shader.use();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    normShader.use();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    // INDEX EBO
    GLuint EBO_INDEX;
    glGenBuffers(1, &EBO_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_INDEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.getIndices().size() * sizeof(int), cube.getIndices().data(), GL_STATIC_DRAW);

    // normal
    GLuint VBO_NORMAL;
    glGenBuffers(1, &VBO_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_NORMAL);
    glBufferData(GL_ARRAY_BUFFER, cube.getNormals().size() * sizeof(float), cube.getNormals().data(), GL_STATIC_DRAW);
    shader.use();
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    normShader.use();
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoord
    GLuint VBO_TEXCOORD;
    glGenBuffers(1, &VBO_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TEXCOORD);
    glBufferData(GL_ARRAY_BUFFER, cube.getTexcoords().size() * sizeof(float), cube.getTexcoords().data(), GL_STATIC_DRAW);
    shader.use();
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(2);
    normShader.use();
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader.use();

    // 纹理绑定
    std::string imagePath = "assets/container.jpg";
    cube.setTexture(imagePath);
    cube.getTexture().bindTexture(shader, "u_texture", 0);

    normShader.use();

    // 变换矩阵
    glm::mat4 modelMtx = glm::mat4(1.0f);
    glm::mat4 viewMtx = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projMtx = glm::perspective<float>(glm::pi<float>() / 3.0, WIDTH * 1.0 / HEIGHT, 1, 1000);

    glLineWidth(4.0);

    // 窗口主循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        // model matrix
        modelMtx = glm::rotate(modelMtx, glm::radians(0.1f), glm::vec3(1.0, 0.0, 0.0)); // 沿x轴旋转0.5度
        modelMtx = glm::rotate(modelMtx, glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0)); // 沿y轴旋转0.5度
        modelMtx = glm::rotate(modelMtx, glm::radians(0.3f), glm::vec3(0.0, 0.0, 1.0)); // 沿z轴旋转0.5度

        // projection matrix 放在循环里，因为WIDTH和HEIGHT可能会变化
        projMtx = glm::perspective<float>(glm::pi<float>() / 3.0, WIDTH * 1.0 / HEIGHT, 1, 1000);

        shader.use();
        shader.setMat4Float("u_modelMtx", glm::value_ptr(modelMtx));
        shader.setMat4Float("u_viewMtx", glm::value_ptr(viewMtx));
        shader.setMat4Float("u_projMtx", glm::value_ptr(projMtx));
        glDrawElements(GL_TRIANGLES, cube.getIndices().size(), GL_UNSIGNED_INT, 0);

        normShader.use();
        normShader.setMat4Float("u_modelMtx", glm::value_ptr(modelMtx));
        normShader.setMat4Float("u_viewMtx", glm::value_ptr(viewMtx));
        normShader.setMat4Float("u_projMtx", glm::value_ptr(projMtx));
        glDrawElements(GL_TRIANGLES, cube.getIndices().size(), GL_UNSIGNED_INT, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error: " << err << std::endl;
        }

        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents();        // 检查并调用事件
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_POSITION);
    glDeleteBuffers(1, &VBO_NORMAL);
    glDeleteBuffers(1, &VBO_TEXCOORD);
    normShader.destroy();
    shader.destroy();

    glfwTerminate();
    return 0;
}