#pragma once

#include <vector>

#include "Vector3.h"
#include "Vector4.h"

class Face{
public:
    Vector3 normal;
    Vector4 vertices[3];
    
    Face(){ }
    Face(Vector4 a, Vector4 b, Vector4 c);
};

class Model{
private:
    
public:
    std::vector<Vector4> vertices;
    std::vector<Face> faces;
    char color;
    
    Model(std::string filename);
};