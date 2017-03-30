#include "Entity.h"

#include "Util.h"

#include <math.h>

static Cube cube;

Powerup::Powerup(float x, float y, float z) : Entity::Entity(x,y,z,0,0,0,1,1,1){
}

void Powerup::update(float time){
    cube.colormap[0] = (((int)(time+140)/30)%7)+1;
    rotation.vals[0] = Util::deg2rad(time * 3);
    rotation.vals[1] = Util::deg2rad(time * 5);
    position.vals[1] = 5 * sin(Util::deg2rad(time*3));
}

void Powerup::render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces){
    auto mv = worldView * getModelWorldMatrix();
    Util::transform(cube, mv, perspective, renderfaces);
}