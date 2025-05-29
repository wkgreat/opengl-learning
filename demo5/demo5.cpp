/*
    纹理
*/
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "opengl-learning demo4", NULL, NULL);
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

    // shader and program
    const char *vertPath = "shader.vs";
    const char *fragPath = "shader.fs";

    Shader shader(vertPath, fragPath);

    shader.use();

    float vertices[] = {
        0.5f, 0.5f, 0.0f, /*color*/ 1.0f, 0.0f, 0.0f, /*texcoord*/ 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, /*color*/ 0.0f, 1.0f, 0.0f, /*texcoord*/ 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, /*color*/ 0.0f, 0.0f, 1.0f, /*texcoord*/ 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f, /*texcoord*/ 0.0f, 1.0f   // 左上
    };

    // 节点索引数据
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO bind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    // color
    // glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(0);
    // texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    unsigned char *data = stbi_load("container.jpg", &imageWidth, &imageHeight, &nChannels, 0);
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

    data = stbi_load("awesomeface.png", &imageWidth, &imageHeight, &nChannels, 0);
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

    // 窗口主循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents();        // 检查并调用事件
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader.destroy();

    glfwTerminate();
    return 0;
}