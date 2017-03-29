#include "World.h"

#include "Primitives.h"
#include "Util.h"
#include "Random.h"

Plane model(3);
Matrix4 plane_transform;

#define WORLDSIZE 5.0

World::World(){
    auto r = Matrix4::rotation_matrix(Util::deg2rad(90), 0, 0);
    auto s = Matrix4::scale_matrix(WORLDSIZE, WORLDSIZE, 1);
    plane_transform = r * s;
}

void World::render(const Matrix4 &worldView, const Matrix4 &perspective, std::vector<Triangle2D> &renderfaces){
    auto mv = worldView * plane_transform;
    Util::transform(model, mv, perspective, renderfaces);
}

void World::generate(std::vector<Entity*> &entities){
    // Generate some random trees in the X-Z plane
    for(int i=0; i<50; i++){
        float x = Random::get_float(-WORLDSIZE/2, WORLDSIZE/2);
        float z = Random::get_float(-WORLDSIZE/2, WORLDSIZE/2);
        
        entities.push_back(new Tree(x, 0, z));
    }
}
