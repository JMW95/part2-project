#pragma once

#include "Model.h"

class Cube : public Model{
public:
    Cube(char color=0);
};

class Tetra : public Model{
public:
    Tetra(char color=0);
};

class Plane : public Model{
public:
    Plane(char color=0);
};