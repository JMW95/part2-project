#include "Demo.h"

#include "Matrix4.h"
#include "Primitives.h"
#include "Util.h"

#include <math.h>
#include <random>

extern char colourtable[][3];

static std::default_random_engine engine(0);
static int rand_int(int a, int b){
    std::uniform_int_distribution<int> rand(a, b);
    return rand(engine);
}

TrianglesDemo::TrianglesDemo(){
    name = "2D Random Triangles";
}

void TrianglesDemo::switch_to(GPU &g){
    // Setup all shades of each colour
    for(int col=0; col<8; col++){
        for(int shade=0; shade<32; shade++){
            g.set_palette_color(col*32 + shade,
                 PIXEL24((colourtable[col][0]*shade)/31,
                    (colourtable[col][1]*shade)/31,
                    (colourtable[col][2]*shade)/31));
        }
    }
    
    triangles.clear();
}

void TrianglesDemo::update(GPU &g, int step){
    if(triangles.size() >= 100){
        triangles.clear();
    }
    
    if(step % 4 == 0){
        // Add a new triangle
        Triangle2D t;
        for(int i=0; i<3; i++){
            t.points[i].x = rand_int(0, 480);
            t.points[i].y = rand_int(0, 272);
        }
        t.color = (rand_int(1,8)*32) - 1;
        triangles.push_back(t);
    }
}

void TrianglesDemo::render(GPU &g, int step){
    // Draw all triangles
    for(auto it = triangles.begin(); it < triangles.end(); ++it){
        g.triangle((*it));
    }
}