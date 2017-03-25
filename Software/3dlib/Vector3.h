#pragma once

#include "Vector4.h"

#include <iostream>

class Vector3{
public:
    float vals[3];
    
    Vector3(float a=0.0f, float b=0.0f, float c=0.0f);
    Vector3(const Vector4 &other);
    
    // Assignment
    void operator=(const Vector3& other);
    
    // Vector addition
    void operator+=(const Vector3& other);
    Vector3 operator+(const Vector3& other);
    
    // Vector subtraction
    void operator-=(const Vector3& other);
    Vector3 operator-(const Vector3& other);
    
    // Scalar multiplication
    void operator*=(const float scale);
    Vector3 operator*(const float scale);
    
    // Scalar division
    void operator/=(const float scale);
    Vector3 operator/(const float scale);
    
    float dot(const Vector3& other);
    Vector3 cross(const Vector3& other);
    float mag();
    Vector3 normalise();
    
};

inline std::ostream &operator<<(std::ostream &Str, Vector3 const &v) { 
    Str << "[";
    for(int i=0; i<3; i++){
        Str << v.vals[i];
        if(i != 2){
            Str << ", ";
        }
    }
    Str << "]";
    return Str;
}