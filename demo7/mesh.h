#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
private:
    std::shared_ptr<stbi_uc> data = nullptr;
    int width;
    int height;

public:
    Texture() : data(nullptr), width(0), height(0) {}
    Texture(std::shared_ptr<stbi_uc> data, int width, int height) : data(data), width(width), height(height) {}
    Texture(std::string path)
    {
        int nChannels;
        stbi_uc *image = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
        std::shared_ptr<stbi_uc> ptr(image, [](stbi_uc *p)
                                     { stbi_image_free(p); });
    }
    ~Texture() = default;

    void setTexture(std::shared_ptr<stbi_uc> data, int width, int height)
    {
        this->data = data;
        this->width = width;
        this->height = height;
    }
    std::shared_ptr<stbi_uc> getData() const
    {
        return this->data;
    }
    int getWidth() const { return this->width; }
    int geiHeight() const { return this->height; }
};

class Mesh
{
protected:
    std::vector<float> positions = {};
    std::vector<uint32_t> indices = {};
    std::vector<float> normals = {};
    std::vector<float> texcoords = {};
    Texture texture;
    bool hasIndex = false;

public:
    Mesh() = default;
    ~Mesh() = default;
    void addPositions(const float *ps, const int n)
    {
        this->positions.insert(this->positions.end(), ps, ps + n);
    }
    void addPositions(std::vector<float> &&ps)
    {
        this->positions = ps;
    }
    void addIndices(const float *is, const int n)
    {
        this->indices.insert(this->indices.end(), is, is + n);
        this->hasIndex = true;
    }
    void addIndices(std::vector<uint32_t> &&is)
    {
        this->indices = is;
    }
    void addNormals(const float *ns, int n)
    {
        this->normals.insert(this->normals.end(), ns, ns + n);
    }
    void addNormals(std::vector<float> &&ns)
    {
        this->normals = ns;
    }
    void addTexcoords(const float *ts, int n)
    {
        this->texcoords.insert(this->texcoords.end(), ts, ts + n);
    }
    void addTexcoords(std::vector<float> &&ts)
    {
        this->texcoords = ts;
    }
    void setTexture(Texture texture)
    {
        this->texture = texture;
    }

    std::vector<float> &getPositions()
    {
        return this->positions;
    }
    std::vector<uint32_t> &getIndices()
    {
        return this->indices;
    }
    std::vector<float> &getNormals()
    {
        return this->normals;
    }
    std::vector<float> &getTexcoords()
    {
        return this->texcoords;
    }
    Texture &getTexture()
    {
        return this->texture;
    }
};

class CubeMesh : public Mesh
{
private:
    void construct()
    {
        this->positions = {
            -0.5, -0.5, 0.5,
            0.5, -0.5, 0.5,
            0.5, 0.5, 0.5,
            -0.5, 0.5, 0.5,
            0.5, -0.5, -0.5,
            -0.5, -0.5, -0.5,
            -0.5, 0.5, -0.5,
            0.5, 0.5, -0.5,
            -0.5, -0.5, -0.5,
            -0.5, -0.5, 0.5,
            -0.5, 0.5, 0.5,
            -0.5, 0.5, -0.5,
            0.5, -0.5, 0.5,
            0.5, -0.5, -0.5,
            0.5, 0.5, -0.5,
            0.5, 0.5, 0.5,
            -0.5, 0.5, 0.5,
            0.5, 0.5, 0.5,
            0.5, 0.5, -0.5,
            -0.5, 0.5, -0.5,
            -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            0.5, -0.5, 0.5,
            -0.5, -0.5, 0.5};

        this->normals = {
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0};

        this->texcoords = {
            0, 0,
            1, 0,
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            1, 1,
            0, 1,
            0, 0,
            1, 0,
            1, 1,
            0, 1};

        this->indices = {
            0, 1, 2, 2, 3, 0,       // 前
            4, 5, 6, 6, 7, 4,       // 后
            8, 9, 10, 10, 11, 8,    // 左
            12, 13, 14, 14, 15, 12, // 右
            16, 17, 18, 18, 19, 16, // 上
            20, 21, 22, 22, 23, 20  // 下
        };
    }

public:
    CubeMesh()
    {
        construct();
    }

    void setTexture(std::string &path)
    {
        this->texture = Texture(path);
    }
};