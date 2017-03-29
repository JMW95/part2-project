#include "Entity.h"

Entity::Entity(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz){
    position = Vector3(x, y, z);
    rotation = Vector3(rx, ry, rz);
    scale = Vector3(sx, sy, sz);
}

Matrix4 Entity::getModelWorldMatrix(){
    auto s = Matrix4::scale_matrix(scale.vals[0], scale.vals[1], scale.vals[2]);
    auto r = Matrix4::rotation_matrix(rotation.vals[0], rotation.vals[1], rotation.vals[2]);
    auto t = Matrix4::translation_matrix(position.vals[0], position.vals[1], position.vals[2]);
    return t * r * s;
};