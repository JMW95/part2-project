#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>

#include <signal.h>
#include <cstring>
#include <atomic>

#include "GPU.h"
#include "Model.h"
#include "Matrix4.h"
#include "Timer.h"

#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

std::atomic<bool> quit(false);

void got_signal(int){
    quit.store(true);
}

static float deg2rad(float degrees){
    return degrees * 3.14159 / 180.0;
}

bool trizcomp(Triangle2D a, Triangle2D b){
    return a.maxdepth < b.maxdepth;
}

char colourtable[][3] = {
    {0,0,0},
    {255,255,255},
    {255,0,0},
    {0,255,0},
    {0,0,255},
    {0,255,255},
    {255,0,255},
    {255,255,0}
};

void transform(const Model &m, Matrix4 &modelView, Matrix4& proj,
                std::vector<Triangle2D> &renderfaces){
    Vector3 camnormal(0,0,1);
    Triangle2D tri;
    auto T = proj * modelView;
    auto NMV = modelView.topleft().inverse().transpose();
    
    for(auto it = m.faces.begin(); it < m.faces.end(); ++it){
        auto f = Face();
        bool valid = true;
        
        f.normal = (NMV * (*it).normal).normalise();
        auto col = camnormal.dot(f.normal);
        if(col < 0) continue; // Back-face culling
        
        float maxdepth = -1e10;
        for(int i=0; i<3; i++){
            auto v = T * (*it).vertices[i];
            v.vals[0] /= v.vals[3];
            v.vals[1] /= v.vals[3];
            v.vals[2] /= v.vals[3];
            if (v.vals[0] < -1 || v.vals[0] > 1 ||
                    v.vals[1] < -1 || v.vals[1] > 1 ||
                    v.vals[2] < 0){ // Vertex is OOB
                i = 2;
                valid = false;
                continue;
            }
            f.vertices[i] = v;
            tri.points[i].x = 480 * ((v.vals[0] + 1) / 2);
            tri.points[i].y = 272 * ((v.vals[1] + 1) / 2);
            tri.depths[i] = v.vals[2];
            if(v.vals[2] > maxdepth){
                maxdepth = v.vals[2];
            }
        }
        
        if(valid){
            tri.color = m.color*32 + (col*32);
            tri.maxdepth = maxdepth;
            renderfaces.push_back(tri);
        }
        
    }
}

int main(int argc, char *argv[]){
    
    // Ctrl-C will cause the main loop to exit cleanly
    struct sigaction sa;
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = got_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    
    
    Model teapot("Teapot.obj");
    Model basicMan("BasicCriypticman.obj");
    
    teapot.color = 2;
    basicMan.color = 3;
    
    GPU g;
    Timer calc, draw, frame, sync;
    
    //Initialise to a black screen
    g.sof();
    g.eof();
    
    for(int col=0; col<8; col++){
        for(int shade=0; shade<32; shade++){
            g.set_palette_color(col*32 + shade,
                 PIXEL24((colourtable[col][0]*shade)/32,
                    (colourtable[col][1]*shade)/32,
                    (colourtable[col][2]*shade)/32));
        }
    }
    
    std::vector<Triangle2D> renderfaces;
    
    int i=0;
    while(!quit.load()){
        
        if(i%180 == 0){
            g.set_use_hardware(i%360 == 0);
        }
        
        frame.start();
        calc.start();
        
        renderfaces.clear();
        
        auto p = Matrix4::perspective_matrix(90, 16.0/9.0, 0.1, 50);
        
        // Teapot
        {
        auto s = Matrix4::scale_matrix(1, 1, 1);
        auto r = Matrix4::rotation_matrix(0, deg2rad(i*3), 0);
        auto t = Matrix4::translation_matrix(-1.5, -1, 30);
        auto mv = t * r * s;
        transform(teapot, mv, p, renderfaces);
        }
        
        // BasicCriypticman
        {
        float sc = 0.6 + 0.4*sin(deg2rad(i*2));
        auto s = Matrix4::scale_matrix(sc, sc, sc);
        auto r = Matrix4::rotation_matrix(0, deg2rad(i*-10), 0);
        auto t = Matrix4::translation_matrix(3, -2.5, 30);
        auto mv = t * r * s;
        transform(basicMan, mv, p, renderfaces);
        }
        
        std::sort(renderfaces.begin(), renderfaces.end(), trizcomp);
        
        calc.stop();
        
        sync.start();
        g.wait_done();
        g.flip();
        g.vsync();
        sync.stop();
        
        draw.start();
        
        g.sof();
        
        for(auto it = renderfaces.begin(); it < renderfaces.end(); ++it){
            g.triangle((*it));
        }
        
        g.eof();
        draw.stop();
        
        frame.stop();
        
        std::cout << "Drew " << renderfaces.size() << " tris\t" << \
            "Calc: " << calc.get_milliseconds() << \
            "ms\tDraw: " << draw.get_milliseconds() << "ms\tTotal: " << \
            (calc.get_milliseconds() + draw.get_milliseconds()) << \
            "ms\tFrame: " << frame.get_milliseconds() << "ms\tFPS: " << \
            floor(1/frame.get_seconds()) << "\tSync: " << \
            sync.get_milliseconds() << "ms" << std::endl;
        
        i++;
    }
    
    std::cout << "Quitting!" << std::endl;
    
}