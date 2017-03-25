#include "Vector4.h"

#include <math.h>

Vector4::Vector4(float a, float b, float c, float d){
    vals[0] = a;
    vals[1] = b;
    vals[2] = c;
    vals[3] = d;
}

float Vector4::dot(const Vector4& other){
    float sum = 0;
    for(int i=0; i<4; i++){
        sum += vals[i] * other.vals[i];
    }
    return sum;
}

float Vector4::mag(){
    float sum = 0;
    for(int i=0; i<4; i++){
        sum += vals[i] * vals[i];
    }
    return sqrt(sum);
}

Vector4 Vector4::normalise(){
    float m = mag();
    Vector4 res = *this;
    return res / m;
}

void Vector4::operator=(const Vector4& other){
    for(int i=0; i<4; i++){
        vals[i] = other.vals[i];
    }
}

void Vector4::operator+=(const Vector4& other){
    for(int i=0; i<4; i++){
        vals[i] += other.vals[i];
    }
}
Vector4 Vector4::operator+(const Vector4& other){
    Vector4 res = *this;
    res += other;
    return res;
}

void Vector4::operator-=(const Vector4& other){
    for(int i=0; i<4; i++){
        vals[i] -= other.vals[i];
    }
}
Vector4 Vector4::operator-(const Vector4& other){
    Vector4 res = *this;
    res -= other;
    return res;
}

void Vector4::operator*=(const float scale){
    for(int i=0; i<4; i++){
        vals[i] *= scale;
    }
}
Vector4 Vector4::operator*(const float scale){
    Vector4 res = *this;
    res *= scale;
    return res;
}

void Vector4::operator/=(const float scale){
    for(int i=0; i<4; i++){
        vals[i] /= scale;
    }
}
Vector4 Vector4::operator/(const float scale){
    Vector4 res = *this;
    res /= scale;
    return res;
}