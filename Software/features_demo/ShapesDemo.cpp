#include "Demo.h"

#include "Matrix4.h"
#include "Primitives.h"
#include "Util.h"

#include <math.h>

extern char colourtable[][3];

ShapesDemo::ShapesDemo(){
    name = "3D Shapes";
    
    selected = 0;
    
    auto man = new Model("models/BasicCriypticman.obj");
    man->colormap[0] = 4;
    
    auto teapot = new Model("models/Teapot.obj");
    teapot->colormap[0] = 3;
    
    models.push_back(new Tetra(1));
    models.push_back(new Cube(2));
    models.push_back(man);
    models.push_back(teapot);
}

void ShapesDemo::switch_to(GPU &g){
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

void ShapesDemo::update(GPU &g, int step){
    
    // Press A to cycle between models
    if(g.was_button_just_pressed(GPU::BUTTON_A)){
        selected ++;
        if(selected >= models.size()){
            selected = 0;
        }
    }
    
    renderfaces.clear();
    
    auto p = Matrix4::perspective_matrix(120, 16.0/9.0, 0.1, 100);
    
    // Render the 3D parts of the scene
    auto cam = Matrix4::scale_matrix(1,1,1);
    
    auto s = Matrix4::scale_matrix(.1, .1, .1);
    auto r = Matrix4::rotation_matrix(Util::deg2rad(2*step), Util::deg2rad(3*step), 0);
    auto t = Matrix4::translation_matrix(0.1*sin(Util::deg2rad(2*step)), 0, 1);
    
    auto model = *models[selected];
    
    if(selected == 2){ // Man
        s = Matrix4::scale_matrix(.04, .04, .04);
    }
    
    if(selected == 3){ // Teapot
        s = Matrix4::scale_matrix(.05, .05, .05);
    }
    
    auto mw = t * r * s;
    
    Util::transform(model, cam, mw, p, renderfaces);
    
    Util::sort_triangles(renderfaces);
}

void ShapesDemo::render(GPU &g, int step){
    
    // Change the background colour
    if(step%100==0){
        int c = (step/100) % 8;
        g.set_palette_color(0, PIXEL24(colourtable[c][0], colourtable[c][1], colourtable[c][2]));
    }
    
    if(step%130==0){
        selected ++;
        if(selected >= models.size()){
            selected = 0;
        }
    }
    
    // Render the 3D shapes
    for(auto it = renderfaces.begin(); it < renderfaces.end(); ++it){
        g.triangle((*it));
    }
}
