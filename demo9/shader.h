#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>

class Shader
{

private:
    unsigned int id;
    unsigned int compileShader(const GLenum shaderType, const char *path);

public:
    Shader(const char *vertexPath, const char *fragPath);
    Shader(const char *vertexPath, const char *geomPath, const char *fragPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4Float(const std::string &name, glm::f32 *data) const;
    void destroy();
    int getId() const { return id; }
};