/*
    封装shader及program
*/
#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragPath)
{
    std::string vertCode;
    std::string fragCode;

    std::ifstream vSahderFile;
    std::ifstream fShaderFile;

    vSahderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vSahderFile.open(vertexPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vSahderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vSahderFile.close();
        fShaderFile.close();
        vertCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char *vShaderCode = vertCode.c_str();
    const char *fShaderCode = fragCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER); // 创建着色器
    glShaderSource(                            // 指定着色器源码
        vertex,                                // 着色器ID
        1,                                     // 字符串数量
        &vShaderCode,                          // 源码
        NULL                                   // 整型数组，指定每个字符串长度；如果为NULL，则每个字符串以NULL结尾
    );
    glCompileShader(vertex);                            // 编译着色器
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // 获取着色器状态
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog); // 如果为成功，获取着色器日志
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // program
    id = glCreateProgram();                       // 创建program
    glAttachShader(id, vertex);                   // 绑定vertex
    glAttachShader(id, fragment);                 // 绑定fragment
    glLinkProgram(id);                            // 链接
    glGetProgramiv(id, GL_LINK_STATUS, &success); // 获取状态
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // 着色器已连接到程序，可以删除
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::use()
{
    glUseProgram(id);
}
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::destroy()
{
    glDeleteProgram(id);
}