#pragma once

#include <vector>

#include "Model.h"
#include "Matrix4.h"
#include "Triangle.h"

class Util{
public:
    static void transform(const Model &m, Matrix4 &modelView, Matrix4& proj,
                std::vector<Triangle2D> &renderfaces);
    static void sort_triangles(std::vector<Triangle2D> &renderfaces);
};