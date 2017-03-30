#pragma once

#include "Vector3.h"
#include "Matrix4.h"

class Camera{
protected:
    Vector3 position;
    Vector3 rotation;
public:
    Camera();
    Matrix4 get_view_matrix();
    Vector3 get_forward_vector();
    Vector3 get_right_vector();
    void move_forward(float dist);
    void move_right(float dist);
    void pitch(float diff);
    void yaw(float diff);
};
