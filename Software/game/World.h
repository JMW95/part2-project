#pragma once

#include "Model.h"
#include "Matrix4.h"
#include "Triangle.h"

class World{
public:
    World();
    void render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces);
};