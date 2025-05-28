/**
 * 使用GLFW绘制一个窗口
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    // glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "opengl-learning demo1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 加载glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置viewport
    glViewport(0, 0, 800, 600);
    // 窗口resize回调
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });

    // 窗口主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents();        // 检查并调用事件
    }

    // 释放资源
    glfwTerminate();
    return 0;
}