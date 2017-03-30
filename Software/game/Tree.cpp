#include "Entity.h"

#include "Util.h"

Model tree("models/lowpolytree.obj", {{"Leaves", 0}, {"Bark", 1}});

Tree::Tree(float x, float y, float z) : Entity::Entity(x,y,z,0,0,0,.1,.1,.1){
    tree.colormap[0] = 7;
    tree.colormap[1] = 6;
}

void Tree::update(float time){
}

void Tree::render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces){
    auto mv = worldView * getModelWorldMatrix();
    Util::transform(tree, mv, perspective, renderfaces);
}