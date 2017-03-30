#include "Camera.h"

#include "Util.h"

#include <math.h>

Camera::Camera(){
    position = Vector3(0,0.1,0);
    rotation = Vector3(0,0,0); // pitch, yaw, roll
}

Matrix4 Camera::get_view_matrix(){
    auto camr = Matrix4::rotation_matrix(-rotation.vals[0], -rotation.vals[1], -rotation.vals[2]);
    auto camt = Matrix4::translation_matrix(-position.vals[0], -position.vals[1], -position.vals[2]);
    return camr * camt;
}

Vector3 Camera::get_forward_vector(){
    float p = rotation.vals[0] + Util::deg2rad(90);
    float t = rotation.vals[1];
    float x = sin(p) * sin(t);
    float y = cos(p);
    float z = sin(p) * cos(t);
    return Vector3(x, y, z);
}

Vector3 Camera::get_right_vector(){
    float t = rotation.vals[1] - Util::deg2rad(90);
    float x = sin(t);
    float z = cos(t);
    return Vector3(x, 0, z);
}

void Camera::move_forward(float dist){
    position += get_forward_vector() * dist;
}

void Camera::move_right(float dist){
    position += get_right_vector() * dist;
}

void Camera::pitch(float diff){
    rotation.vals[0] += diff;
}

void Camera::yaw(float diff){
    rotation.vals[1] += diff;
}
