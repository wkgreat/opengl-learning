/*
    封装shader及program
*/
#include "shader.h"

unsigned int Shader::compileShader(const GLenum shaderType, const char *path)
{
    std::string code;
    std::ifstream fs;

    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fs.open(path);
        std::stringstream ss;
        ss << fs.rdbuf();
        fs.close();
        code = ss.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char *codestr = code.c_str();

    unsigned int shader;
    int success;
    char infoLog[512];

    // vertex shader
    shader = glCreateShader(shaderType); // 创建着色器
    glShaderSource(                      // 指定着色器源码
        shader,                          // 着色器ID
        1,                               // 字符串数量
        &codestr,                        // 源码
        NULL                             // 整型数组，指定每个字符串长度；如果为NULL，则每个字符串以NULL结尾
    );
    glCompileShader(shader);                            // 编译着色器
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // 获取着色器状态
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); // 如果为成功，获取着色器日志
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return shader;
}

Shader::Shader(const char *vertexPath, const char *fragPath)
{

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragPath);
    char infoLog[512];
    int success;

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

Shader::Shader(const char *vertexPath, const char *geomPath, const char *fragPath)
{

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int geometry = -1;
    if (geomPath)
    {
        geometry = compileShader(GL_GEOMETRY_SHADER, geomPath);
    }
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragPath);
    char infoLog[512];
    int success;

    // program
    id = glCreateProgram();     // 创建program
    glAttachShader(id, vertex); // 绑定vertex
    if (geometry >= 0)
    {
        glAttachShader(id, geometry);
    }
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
    if (geometry >= 0)
    {
        glDeleteShader(geometry);
    }
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

void Shader::setMat4Float(const std::string &name, glm::f32 *data) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, static_cast<GLfloat *>(data));
}

void Shader::destroy()
{
    glDeleteProgram(id);
}