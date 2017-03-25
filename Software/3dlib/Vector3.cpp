#include "Vector3.h"

#include <math.h>

Vector3::Vector3(float a, float b, float c){
    vals[0] = a;
    vals[1] = b;
    vals[2] = c;
}

Vector3::Vector3(Vector4 const &other){
    for(int i=0; i<3; i++){
        vals[i] = other.vals[i];
    }
}

float Vector3::dot(const Vector3& other){
    float sum = 0;
    for(int i=0; i<3; i++){
        sum += vals[i] * other.vals[i];
    }
    return sum;
}

Vector3 Vector3::cross(const Vector3& other){
    Vector3 res;
    res.vals[0] = vals[1]*other.vals[2] - vals[2]*other.vals[1];
    res.vals[1] = vals[2]*other.vals[0] - vals[0]*other.vals[2];
    res.vals[2] = vals[0]*other.vals[1] - vals[1]*other.vals[0];
    return res;
}

float Vector3::mag(){
    float sum = 0;
    for(int i=0; i<3; i++){
        sum += vals[i] * vals[i];
    }
    return sqrt(sum);
}

Vector3 Vector3::normalise(){
    float m = mag();
    Vector3 res = *this;
    return res / m;
}

void Vector3::operator=(const Vector3& other){
    for(int i=0; i<3; i++){
        vals[i] = other.vals[i];
    }
}

void Vector3::operator+=(const Vector3& other){
    for(int i=0; i<3; i++){
        vals[i] += other.vals[i];
    }
}
Vector3 Vector3::operator+(const Vector3& other){
    Vector3 res = *this;
    res += other;
    return res;
}

void Vector3::operator-=(const Vector3& other){
    for(int i=0; i<3; i++){
        vals[i] -= other.vals[i];
    }
}
Vector3 Vector3::operator-(const Vector3& other){
    Vector3 res = *this;
    res -= other;
    return res;
}

void Vector3::operator*=(const float scale){
    for(int i=0; i<3; i++){
        vals[i] *= scale;
    }
}
Vector3 Vector3::operator*(const float scale){
    Vector3 res = *this;
    res *= scale;
    return res;
}

void Vector3::operator/=(const float scale){
    for(int i=0; i<3; i++){
        vals[i] /= scale;
    }
}
Vector3 Vector3::operator/(const float scale){
    Vector3 res = *this;
    res /= scale;
    return res;
}