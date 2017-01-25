#pragma once

#include <iostream>

class Vector4{
public:
    float vals[4];
    
    Vector4(float a=0.0f, float b=0.0f, float c=0.0f, float d=0.0f);
    
    // Assignment
    void operator=(const Vector4& other);
    
    // Vector addition
    void operator+=(const Vector4& other);
    Vector4 operator+(const Vector4& other);
    
    // Vector subtraction
    void operator-=(const Vector4& other);
    Vector4 operator-(const Vector4& other);
    
    // Scalar multiplication
    void operator*=(const float scale);
    Vector4 operator*(const float scale);
    
    // Scalar division
    void operator/=(const float scale);
    Vector4 operator/(const float scale);
    
    float dot(const Vector4& other);
    float mag();
    Vector4 normalise();
    
};

inline std::ostream &operator<<(std::ostream &Str, Vector4 const &v) { 
    Str << "[";
    for(int i=0; i<4; i++){
        Str << v.vals[i];
        if(i != 3){
            Str << ", ";
        }
    }
    Str << "]";
    return Str;
}