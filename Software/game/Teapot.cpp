#include "Entity.h"

#include "Util.h"

Model teapot("models/Teapot.obj");

Teapot::Teapot(float x, float y, float z) : Entity::Entity(x,y,z,0,0,0,0.5,0.5,0.5){
}

void Teapot::update(float time){
    teapot.color = ((int)(time/30)%7)+1;
    rotation.vals[1] = Util::deg2rad(time * 3);
}

void Teapot::render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces){
    auto mv = worldView * getModelWorldMatrix();
    Util::transform(teapot, mv, perspective, renderfaces);
}