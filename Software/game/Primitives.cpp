#include "Primitives.h"

Cube::Cube(char color){
    this->color = color;
    
    vertices.push_back(Vector4(-0.5,-0.5,-0.5,1));
    vertices.push_back(Vector4(-0.5,-0.5, 0.5,1));
    vertices.push_back(Vector4( 0.5,-0.5,-0.5,1));
    vertices.push_back(Vector4( 0.5,-0.5, 0.5,1));
    vertices.push_back(Vector4(-0.5, 0.5,-0.5,1));
    vertices.push_back(Vector4(-0.5, 0.5, 0.5,1));
    vertices.push_back(Vector4( 0.5, 0.5,-0.5,1));
    vertices.push_back(Vector4( 0.5, 0.5, 0.5,1));
    
    // Back
    _face(3, 1, 0);
    _face(2, 3, 0);
    
    // Front
    _face(5, 7, 6);
    _face(4, 5, 6);
    
    // Left
    _face(0, 1, 5);
    _face(4, 0, 5);
    
    // Right
    _face(7, 3, 2);
    _face(6, 7, 2);
    
    // Top
    _face(7, 5, 1);
    _face(3, 7, 1);
    
    // Bottom
    _face(6, 2, 0);
    _face(4, 6, 0);
}

Tetra::Tetra(char color){
    this->color = color;
    
    vertices.push_back(Vector4( 0.5,  0.5,  0.5, 1));
    vertices.push_back(Vector4(-0.5, -0.5,  0.5, 1));
    vertices.push_back(Vector4( 0.5, -0.5, -0.5, 1));
    vertices.push_back(Vector4(-0.5,  0.5, -0.5, 1));
    
    _face(0, 1, 3);
    _face(0, 3, 2);
    _face(0, 2, 1);
    _face(1, 2, 3);
}

Plane::Plane(char color){
    this->color = color;
    
    vertices.push_back(Vector4( 0.5,  0.5,  0, 1));
    vertices.push_back(Vector4(-0.5,  0.5,  0, 1));
    vertices.push_back(Vector4( 0.5, -0.5,  0, 1));
    vertices.push_back(Vector4(-0.5, -0.5,  0, 1));
    
    _face(1, 0, 2);
    _face(1, 2, 3);
}
