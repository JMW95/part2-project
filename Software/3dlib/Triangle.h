#pragma once

#include <iostream>

struct Point2D {
    short x;
    short y;
};

struct Triangle2D{
    struct Point2D points[3];
    float depths[3];
    float mindepth;
    short color;
};

inline std::ostream &operator<<(std::ostream &Str, const Triangle2D &v) {
    Str << "[";
    for(int i=0; i<3; i++){
        Str << "(" << v.points[i].x << ", " << v.points[i].y << ")";
        if(i != 2){
            Str << ", ";
        }
    }
    Str << "]";
    return Str;
}