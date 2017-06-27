#pragma once

#include <string>

#include "GPU.h"
#include "Model.h"
#include "Triangle.h"

#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

class Demo {
private:
public:
    std::string name;
    virtual void update(GPU &g, int step) = 0;
    virtual void render(GPU &g, int step) = 0;
    virtual void switch_to(GPU &g) = 0;
};

class ShapesDemo : public Demo {
private:
    std::vector<Triangle2D> renderfaces;
    std::vector<Model*> models;
    unsigned int selected;
public:
    ShapesDemo();
    void switch_to(GPU &g);
    void update(GPU &g, int step);
    void render(GPU &g, int step);
};

class SceneDemo : public Demo {
private:
    std::vector<Triangle2D> renderfaces;
public:
    SceneDemo();
    void switch_to(GPU &g);
    void update(GPU &g, int step);
    void render(GPU &g, int step);
};