#include <iostream>
#include <math.h>
#include <time.h>

#include <signal.h>
#include <cstring>
#include <atomic>

#include "GPU.h"
#include "Util.h"
#include "Timer.h"

#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

std::atomic<bool> quit(false);

void got_signal(int){
    quit.store(true);
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

int main(int argc, char *argv[]){
    
    // Ctrl-C will cause the main loop to exit cleanly
    struct sigaction sa;
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = got_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    
    Model teapot("Teapot.obj", 2);
    Model basicMan("BasicCriypticman.obj", 3);
    
    GPU g;
    Timer calc, draw, frame, sync;

    g.set_use_hardware(true);
    
    std::cout << "GPU has " << g.get_num_cores() << " cores." << std::endl;
    
    //Initialise to a black screen
    g.sof();
    g.eof();
    
    // Setup all shades of each colour
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
            //g.set_use_hardware(i%360 == 0);
            //std::cout << "using hardware? : " << (i%360 == 0) << std::endl;
        }
        
        frame.start();
        calc.start();
        
        renderfaces.clear();
        
        auto p = Matrix4::perspective_matrix(90, 16.0/9.0, 0.1, 50);
        
        // Teapot
        {
        teapot.color = ((i/30)%7)+1;
        auto s = Matrix4::scale_matrix(2, 2, 2);
        auto r = Matrix4::rotation_matrix(0, Util::deg2rad(i*3), 0);
        auto t = Matrix4::translation_matrix(-1.5, -1, 30);
        auto mv = t * r * s;
        Util::transform(teapot, mv, p, renderfaces);
        }
        
        // BasicCriypticman
        {
        basicMan.color = (((i+75)/30)%7)+1;
        float sc = 0.6 + 0.4*sin(Util::deg2rad(i*2));
        auto s = Matrix4::scale_matrix(sc, sc, sc);
        auto r = Matrix4::rotation_matrix(0, Util::deg2rad(i*-10), 0);
        auto t = Matrix4::translation_matrix(3, -2.5, 30);
        auto mv = t * r * s;
        Util::transform(basicMan, mv, p, renderfaces);
        }
        
        Util::sort_triangles(renderfaces);
        
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
