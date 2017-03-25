#include "Matrix4.h"
#include <math.h>

Matrix4::Matrix4(float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float m, float n, float o, float p){
    vals[0][0] = a;
    vals[0][1] = b;
    vals[0][2] = c;
    vals[0][3] = d;
    vals[1][0] = e;
    vals[1][1] = f;
    vals[1][2] = g;
    vals[1][3] = h;
    vals[2][0] = i;
    vals[2][1] = j;
    vals[2][2] = k;
    vals[2][3] = l;
    vals[3][0] = m;
    vals[3][1] = n;
    vals[3][2] = o;
    vals[3][3] = p;
}

static float deg2rad(float degrees){
    return degrees * 3.14159 / 180.0;
}

Matrix4 Matrix4::translation_matrix(float x, float y, float z){
    Matrix4 m;
    m.vals[0][0] = 1;
    m.vals[0][3] = x;
    m.vals[1][1] = 1;
    m.vals[1][3] = y;
    m.vals[2][2] = 1;
    m.vals[2][3] = z;
    m.vals[3][3] = 1;
    return m;
}

Matrix4 Matrix4::rotation_matrix(float x, float y, float z){
    Matrix4 rx;
    rx.vals[0][0] = 1;
    rx.vals[1][1] = cos(x);
    rx.vals[1][2] = -sin(x);
    rx.vals[2][1] = sin(x);
    rx.vals[2][2] = cos(x);
    rx.vals[3][3] = 1;
    
    Matrix4 ry;
    ry.vals[0][0] = cos(y);
    ry.vals[0][2] = sin(y);
    ry.vals[1][1] = 1;
    ry.vals[2][0] = -sin(y);
    ry.vals[2][2] = cos(y);
    ry.vals[3][3] = 1;
    
    Matrix4 rz;
    rz.vals[0][0] = cos(z);
    rz.vals[0][1] = -sin(z);
    rz.vals[1][0] = sin(z);
    rz.vals[1][1] = cos(z);
    rz.vals[2][2] = 1;
    rz.vals[3][3] = 1;
    
    return rx * ry * rz;
}

Matrix4 Matrix4::scale_matrix(float x, float y, float z){
    Matrix4 m;
    m.vals[0][0] = x;
    m.vals[1][1] = y;
    m.vals[2][2] = z;
    m.vals[3][3] = 1;
    return m;
}

Matrix4 Matrix4::perspective_matrix(float fovy, float aspect, float near, float far){
    float f = 1.0 / tan(deg2rad(fovy/2));
    Matrix4 m;
    m.vals[0][0] = f / aspect;
    m.vals[1][1] = f;
    m.vals[2][2] = -far / (far - near);
    m.vals[2][3] = -1;
    m.vals[3][2] = -(far*near) / (far-near);
    return m;
}

Matrix4 Matrix4::transpose() const{
    Matrix4 res;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            res.vals[i][j] = vals[j][i];
        }
    }
    return res;
}

Matrix3 Matrix4::topleft() const{
    Matrix3 res;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            res.vals[i][j] = vals[i][j];
        }
    }
    return res;
}

void Matrix4::operator=(const Matrix4& other){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] = other.vals[i][j];
        }
    }
}

void Matrix4::operator+=(const Matrix4& other){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] += other.vals[i][j];
        }
    }
}
Matrix4 Matrix4::operator+(const Matrix4& other) const{
    Matrix4 res = *this;
    res += other;
    return res;
}

void Matrix4::operator-=(const Matrix4& other){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] -= other.vals[i][j];
        }
    }
}
Matrix4 Matrix4::operator-(const Matrix4& other) const{
    Matrix4 res = *this;
    res -= other;
    return res;
}

void Matrix4::operator*=(const Matrix4& other){
    Matrix4 res;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            float tmp = 0;
            for(int k=0; k<4; k++){
                tmp += vals[i][k] * other.vals[k][j];
            }
            res.vals[i][j] = tmp;
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] = res.vals[i][j];
        }
    }
}
Matrix4 Matrix4::operator*(const Matrix4& other) const{
    Matrix4 res = *this;
    res *= other;
    return res;
}

Vector4 Matrix4::operator*(const Vector4& other) const{
    Vector4 res;
    for(int i=0; i<4; i++){
        float tmp = 0;
        for(int k=0; k<4; k++){
            tmp += vals[i][k] * other.vals[k];
        }
        res.vals[i] = tmp;
    }
    return res;
}

void Matrix4::operator*=(const float scale){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] *= scale;
        }
    }
}
Matrix4 Matrix4::operator*(const float scale) const{
    Matrix4 res = *this;
    res *= scale;
    return res;
}

void Matrix4::operator/=(const float scale){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vals[i][j] /= scale;
        }
    }
}
Matrix4 Matrix4::operator/(const float scale) const{
    Matrix4 res = *this;
    res /= scale;
    return res;
}