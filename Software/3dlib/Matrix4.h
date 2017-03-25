#pragma once

#include "Matrix3.h"
#include "Vector4.h"

class Matrix4{
public:
    float vals[4][4];
    
    Matrix4(float a=0.0f, float b=0.0f, float c=0.0f, float d=0.0f,
            float e=0.0f, float f=0.0f, float g=0.0f, float h=0.0f,
            float i=0.0f, float j=0.0f, float k=0.0f, float l=0.0f,
            float m=0.0f, float n=0.0f, float o=0.0f, float p=0.0f);
    
    static Matrix4 translation_matrix(float x, float y, float z);
    static Matrix4 rotation_matrix(float x, float y, float z);
    static Matrix4 scale_matrix(float x, float y, float z);
    static Matrix4 perspective_matrix(float fovy, float aspect, float near, float far);
    
    // Assignment
    void operator=(const Matrix4& other);
    
    // Matrix addition
    void operator+=(const Matrix4& other);
    Matrix4 operator+(const Matrix4& other) const;
    
    // Matrix subtraction
    void operator-=(const Matrix4& other);
    Matrix4 operator-(const Matrix4& other) const;
    
    // Matrix multiplication
    void operator*=(const Matrix4& other);
    Matrix4 operator*(const Matrix4& other) const;
    
    // Vector multiplication
    Vector4 operator*(const Vector4& other) const;
    
    // Scalar multiplication
    void operator*=(const float scale);
    Matrix4 operator*(const float scale) const;
    
    // Scalar division
    void operator/=(const float scale);
    Matrix4 operator/(const float scale) const;

    Matrix4 transpose() const;
    Matrix3 topleft() const;
};

inline std::ostream & operator<<(std::ostream & Str, Matrix4 const & v) { 
    Str << "[";
    for(int i=0; i<4; i++){
        Str << "[";
        for(int j=0; j<4; j++){
            Str << v.vals[i][j];
            if(j != 3){
                Str << ", ";
            }
        }
        Str << "]";
        if(i != 3){
            Str << "\n ";
        }
    }
    Str << "]";
    return Str;
}