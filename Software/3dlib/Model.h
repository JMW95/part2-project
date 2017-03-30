#pragma once

#include <vector>
#include <map>
#include <string>

#include "Vector3.h"
#include "Vector4.h"

class Face{
public:
    Vector3 normal;
    Vector4 vertices[3];
    unsigned char color_index;
    
    Face(){ }
    Face(Vector4 a, Vector4 b, Vector4 c, unsigned char cid);
};

class Model{
protected:
    void _face(int a, int b, int c, unsigned char cid);
public:
    std::vector<Vector4> vertices;
    std::vector<Face> faces;
    
    std::vector<char> colormap;
    
    Model();
    Model(std::string filename, const std::map<std::string, unsigned char> texturemap = std::map<std::string, unsigned char>());
};