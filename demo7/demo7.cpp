/*
    坐标系统
*/
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include "mesh.h"

const int WIDTH = 800;
const int HEIGHT = 600;

GLFWwindow *glInitWindow()
{

    // glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw窗口
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "opengl-learning demo6", NULL, NULL);
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
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });

    return window;
}

int main()
{

    GLFWwindow *window = glInitWindow();

    // GL配置
    glEnable(GL_DEPTH_TEST);

    // shader and program
    const char *vertPath = "shader.vs";
    const char *fragPath = "shader.fs";

    Shader shader(vertPath, fragPath);

    shader.use();

    CubeMesh cube;
    // std::string imagePath = "assets/awesomeface.png";
    // cube.setTexture(imagePath);

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // POSITION VBO
    GLuint VBO_POSITION;
    glGenBuffers(1, &VBO_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);

    glBufferData(GL_ARRAY_BUFFER, cube.getPositions().size() * sizeof(float), cube.getPositions().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    // INDEX EBO
    GLuint EBO_INDEX;
    glGenBuffers(1, &EBO_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_INDEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.getIndices().size() * sizeof(int), cube.getIndices().data(), GL_STATIC_DRAW);

    // texcoord
    GLuint VBO_TEXCOORD;
    glGenBuffers(1, &VBO_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TEXCOORD);
    glBufferData(GL_ARRAY_BUFFER, cube.getTexcoords().size() * sizeof(float), cube.getTexcoords().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 纹理1
    unsigned int texture1;
    glGenTextures(1, &texture1);            // 生成纹理缓冲
    glActiveTexture(GL_TEXTURE0);           // 激活纹理单元0
    glBindTexture(GL_TEXTURE_2D, texture1); // 绑定纹理
    // 纹理设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 读取纹理数据并设置
    int imageWidth, imageHeight, nChannels;
    unsigned char *data = stbi_load("assets/container.jpg", &imageWidth, &imageHeight, &nChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // 纹理2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);           // 激活纹理单元1
    glBindTexture(GL_TEXTURE_2D, texture2); // 绑定texture2
    // 纹理设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("assets/awesomeface.png", &imageWidth, &imageHeight, &nChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // glUniform1i(glGetUniformLocation(shader.getId(), "u_texture1"), 0);
    shader.setInt("u_texture1", 0); // u_texture1使用纹理单元1
    // glUniform1i(glGetUniformLocation(shader.getId(), "u_texture2"), 1);
    shader.setInt("u_texture2", 1); // u_texture2使用纹理单元2

    // 矩阵变换
    glm::mat4 modelMtx = glm::mat4(1.0f);
    // 获取u_modelMtx位置
    unsigned int u_modelMtx = glGetUniformLocation(shader.getId(), "u_modelMtx");

    // 窗口主循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        // 沿x轴旋转0.5度
        modelMtx = glm::rotate(modelMtx, glm::radians(0.5f), glm::vec3(1.0, 0.0, 0.0));
        // 沿y轴旋转0.5度
        modelMtx = glm::rotate(modelMtx, glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0));
        // 沿z轴旋转0.5度
        modelMtx = glm::rotate(modelMtx, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0));
        // 设置获取u_modelMtx
        glUniformMatrix4fv(u_modelMtx, 1, GL_FALSE, glm::value_ptr(modelMtx));

        // glDrawArrays(GL_TRIANGLES, 0, cube.getPositions().size());
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
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO_POSITION);
    // // glDeleteBuffers(1, &VBO_TEXCOORD);
    // shader.destroy();

    glfwTerminate();
    return 0;
}