#include "World.h"

#include "Primitives.h"
#include "Util.h"

Plane model(3);
Matrix4 plane_transform;

World::World(){
    auto r = Matrix4::rotation_matrix(Util::deg2rad(90), 0, 0);
    auto s = Matrix4::scale_matrix(10,10,10);
    plane_transform = r * s;
}

void World::render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces){
    auto mv = worldView * plane_transform;
    Util::transform(model, mv, perspective, renderfaces);
}