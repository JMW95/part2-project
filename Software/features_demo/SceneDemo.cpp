#include "Demo.h"

#include "Matrix4.h"
#include "Primitives.h"
#include "Util.h"

#include <math.h>
#include <random>

#define NUMTREES    30
#define WORLDSIZE   3.0

struct TreePos {
    float x;
    float y;
} y;

static std::default_random_engine engine(0);
static float rand_float(float a, float b){
    std::uniform_real_distribution<float> rand(a, b);
    return rand(engine);
}

static TreePos trees[NUMTREES];

extern char colourtable[][3];

Model tree("models/lowpolytree.obj", {{"Leaves", 0}, {"Bark", 1}});

SceneDemo::SceneDemo(){
    name = "3D Forest";
    
    tree.colormap[0] = 1;
    tree.colormap[1] = 6;
    
    for(int i=0; i<NUMTREES; i++){
        float x = rand_float(-WORLDSIZE/2, WORLDSIZE/2);
        float z = rand_float(-WORLDSIZE/2, WORLDSIZE/2);
        
        trees[i] = {x, z};
    }
}

void SceneDemo::switch_to(GPU &g){
    // Setup all shades of each colour
    for(int col=0; col<8; col++){
        for(int shade=0; shade<32; shade++){
            g.set_palette_color(col*32 + shade,
                 PIXEL24((colourtable[col][0]*shade)/31,
                    (colourtable[col][1]*shade)/31,
                    (colourtable[col][2]*shade)/31));
        }
    }
}

Cube cube(1);

void SceneDemo::update(GPU &g, int step){
    renderfaces.clear();
    
    auto p = Matrix4::perspective_matrix(120, 16.0/9.0, 0.1, 100);
    
    // Render the 3D parts of the scene
    float posyaw = Util::deg2rad(step + 180);
    float dist = 5;
    float x = sin(posyaw) * dist;
    float y = 2;
    float z = cos(posyaw) * dist;
    
    float pitch = Util::deg2rad(20);
    float yaw = Util::deg2rad(step);
    
    auto r = Matrix4::rotation_matrix(-pitch,-yaw,0);
    auto t = Matrix4::translation_matrix(-x, -y, -z);
    auto cam = r * t;
    
    // Render trees
    auto s = Matrix4::scale_matrix(.1, .1, .1);
    for(int i=0; i<NUMTREES; i++){
        auto t = Matrix4::translation_matrix(trees[i].x, 0, trees[i].y);
        Util::transform(tree, cam, t * s, p, renderfaces);
    }
    
    Util::sort_triangles(renderfaces);
}

void SceneDemo::render(GPU &g, int step){
    
    // Render the 3D shapes
    for(auto it = renderfaces.begin(); it < renderfaces.end(); ++it){
        g.triangle((*it));
    }
}