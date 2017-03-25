#pragma once

#include <iostream>

#include "Vector3.h"

class Matrix3{
public:
    float vals[3][3];
    
    Matrix3(float a=0.0f, float b=0.0f, float c=0.0f,
            float d=0.0f, float e=0.0f, float f=0.0f,
            float g=0.0f, float h=0.0f, float i=0.0f);
    
    // Assignment
    void operator=(const Matrix3& other);
    
    // Matrix addition
    void operator+=(const Matrix3& other);
    Matrix3 operator+(const Matrix3& other);
    
    // Matrix subtraction
    void operator-=(const Matrix3& other);
    Matrix3 operator-(const Matrix3& other);
    
    // Matrix multiplication
    void operator*=(const Matrix3& other);
    Matrix3 operator*(const Matrix3& other);
    
    // Vector multiplication
    Vector3 operator*(const Vector3& other);
    
    // Scalar multiplication
    void operator*=(const float scale);
    Matrix3 operator*(const float scale);
    
    // Scalar division
    void operator/=(const float scale);
    Matrix3 operator/(const float scale);
    
    float det();
    Matrix3 cofactors();
    Matrix3 inverse();
    Matrix3 transpose();
};

inline std::ostream &operator<<(std::ostream &Str, Matrix3 const &v) { 
    Str << "[";
    for(int i=0; i<3; i++){
        Str << "[";
        for(int j=0; j<3; j++){
            Str << v.vals[i][j];
            if(j != 2){
                Str << ", ";
            }
        }
        Str << "]";
        if(i != 2){
            Str << "\n ";
        }
    }
    Str << "]";
    return Str;
}