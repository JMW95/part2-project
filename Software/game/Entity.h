#pragma once

#include "Matrix4.h"
#include "Triangle.h"
#include <vector>

#include "Primitives.h"

class Entity{
protected:
    Entity(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz);
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    virtual void update(float time) = 0;
    virtual void render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces) = 0;
    virtual Matrix4 getModelWorldMatrix();
};

class Powerup : public Entity{
public:
    Powerup(float x, float y, float z);
    void update(float time);
    void render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces);
};

class Teapot : public Entity{
public:
    Teapot(float x, float y, float z);
    void update(float time);
    void render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces);
};

class Tree : public Entity{
public:
    Tree(float x, float y, float z);
    void update(float time);
    void render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces);
};