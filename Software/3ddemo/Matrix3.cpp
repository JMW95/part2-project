#include "Matrix3.h"

Matrix3::Matrix3(float a, float b, float c,
            float d, float e, float f,
            float g, float h, float i){
    vals[0][0] = a;
    vals[0][1] = b;
    vals[0][2] = c;
    vals[1][0] = d;
    vals[1][1] = e;
    vals[1][2] = f;
    vals[2][0] = g;
    vals[2][1] = h;
    vals[2][2] = i;
}

float Matrix3::det(){
    float res = 0;
    res += vals[0][0] * (vals[1][1] * vals[2][2] - vals[1][2] * vals[2][1]);
    res -= vals[0][1] * (vals[1][0] * vals[2][2] - vals[1][2] * vals[2][0]);
    res += vals[0][2] * (vals[1][0] * vals[2][1] - vals[1][1] * vals[2][0]);
    return res;
}

Matrix3 Matrix3::cofactors(){
    Matrix3 res;
    res.vals[0][0] = vals[1][1] * vals[2][2] - vals[1][2] * vals[2][1];
    res.vals[0][1] = -(vals[1][0] * vals[2][2] - vals[1][2] * vals[2][0]);
    res.vals[0][2] = vals[1][0] * vals[2][1] - vals[1][1] * vals[2][0];
    
    res.vals[1][0] = -(vals[0][1] * vals[2][2] - vals[0][2] * vals[2][1]);
    res.vals[1][1] = vals[0][0] * vals[2][2] - vals[0][2] * vals[2][0];
    res.vals[1][2] = -(vals[0][0] * vals[2][1] - vals[0][1] * vals[2][0]);
    
    res.vals[2][0] = vals[0][1] * vals[1][2] - vals[0][2] * vals[1][1];
    res.vals[2][1] = -(vals[0][0] * vals[1][2] - vals[0][2] * vals[1][0]);
    res.vals[2][2] = vals[0][0] * vals[1][1] - vals[0][1] * vals[1][0];
    return res;
}

Matrix3 Matrix3::inverse(){
    auto cof = cofactors();
    auto adj = cof.transpose();
    return adj / det();
}

Matrix3 Matrix3::transpose(){
    Matrix3 res;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            res.vals[i][j] = vals[j][i];
        }
    }
    return res;
}

void Matrix3::operator=(const Matrix3& other){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] = other.vals[i][j];
        }
    }
}

void Matrix3::operator+=(const Matrix3& other){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] += other.vals[i][j];
        }
    }
}
Matrix3 Matrix3::operator+(const Matrix3& other){
    Matrix3 res = *this;
    res += other;
    return res;
}

void Matrix3::operator-=(const Matrix3& other){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] -= other.vals[i][j];
        }
    }
}
Matrix3 Matrix3::operator-(const Matrix3& other){
    Matrix3 res = *this;
    res -= other;
    return res;
}

void Matrix3::operator*=(const Matrix3& other){
    Matrix3 res;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            float tmp = 0;
            for(int k=0; k<3; k++){
                tmp += vals[i][k] * other.vals[k][j];
            }
            res.vals[i][j] = tmp;
        }
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] = res.vals[i][j];
        }
    }
}
Matrix3 Matrix3::operator*(const Matrix3& other){
    Matrix3 res = *this;
    res *= other;
    return res;
}

Vector3 Matrix3::operator*(const Vector3& other){
    Vector3 res;
    for(int i=0; i<3; i++){
        float tmp = 0;
        for(int k=0; k<3; k++){
            tmp += vals[i][k] * other.vals[k];
        }
        res.vals[i] = tmp;
    }
    return res;
}

void Matrix3::operator*=(const float scale){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] *= scale;
        }
    }
}
Matrix3 Matrix3::operator*(const float scale){
    Matrix3 res = *this;
    res *= scale;
    return res;
}

void Matrix3::operator/=(const float scale){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            vals[i][j] /= scale;
        }
    }
}
Matrix3 Matrix3::operator/(const float scale){
    Matrix3 res = *this;
    res /= scale;
    return res;
}