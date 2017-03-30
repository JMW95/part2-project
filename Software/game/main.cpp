#include <iostream>
#include <math.h>
#include <time.h>

#include <signal.h>
#include <cstring>
#include <atomic>

#include "GPU.h"
#include "Util.h"
#include "Timer.h"

#include "Primitives.h"
#include "Entity.h"
#include "World.h"
#include "Camera.h"

#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

std::atomic<bool> quit(false);

void got_signal(int){
    quit.store(true);
}

char colourtable[][3] = {
    {0,0,0},        // Black
    {155,105,210},  // Purple
    {255,0,0},      // Red
    {0,255,0},      // Green
    {190,190,190},  // Light Grey
    {150,150,150},  // Dark Grey
    {127,51,0},     // Brown
    {0,127,14}      // Dark Green
};

int main(int argc, char *argv[]){
    
    // Ctrl-C will cause the main loop to exit cleanly
    struct sigaction sa;
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = got_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    
    GPU g;
    Timer calc, draw, frame, sync;

    g.set_use_hardware(false);
    
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
    std::vector<Entity*> entities;
    World world;
    Camera camera;
    
    world.generate(entities);
    
    Model castle("models/castle.obj", {{"lambert9SG", 0}, {"lambert10SG", 1}, {"lambert11SG", 2}, {"lambert12SG", 3}});
    castle.colormap[0] = 5; // Dark grey
    castle.colormap[1] = 4; // Light grey
    castle.colormap[2] = 2; // Red
    castle.colormap[3] = 1; // Purple
    
    int i=0;
    while(!quit.load()){
        frame.start();
        calc.start();
        
        renderfaces.clear();
        
        auto p = Matrix4::perspective_matrix(120, 16.0/9.0, 0.1, 100);
        
        g.read_buttons();
        
        // Update camera position
        if(g.is_button_pressed(GPU::BUTTON_Y)){
            camera.move_forward(0.03);
        }else if(g.is_button_pressed(GPU::BUTTON_A)){
            camera.move_forward(-0.03);
        }
        if(g.is_button_pressed(GPU::BUTTON_X)){
            camera.move_right(-0.03);
        }else if(g.is_button_pressed(GPU::BUTTON_B)){
            camera.move_right(0.03);
        }
        
        if(g.is_button_pressed(GPU::NAV_U)){
            camera.pitch(Util::deg2rad(-1));
        }else if(g.is_button_pressed(GPU::NAV_D)){
            camera.pitch(Util::deg2rad(1));
        }
        if(g.is_button_pressed(GPU::NAV_L)){
            camera.yaw(Util::deg2rad(1));
        }else if(g.is_button_pressed(GPU::NAV_R)){
            camera.yaw(Util::deg2rad(-1));
        }
        
        auto cam = camera.get_view_matrix();
        
        // Render world
        world.render(cam, p, renderfaces);
        
        // Castle
        {
        auto s = Matrix4::scale_matrix(.1, .1, .1);
        auto r = Matrix4::rotation_matrix(0, Util::deg2rad(180), 0);
        auto t = Matrix4::translation_matrix(0, 0, 10);
        auto mv = t * r * s;
        Util::transform(castle, cam * mv, p, renderfaces);
        }
        
        // Update and render entities
        for(auto it = entities.begin(); it < entities.end(); ++it){
            (*it)->update(i);
            (*it)->render(cam, p, renderfaces);
        }
        
        Util::sort_triangles(renderfaces);
        
        calc.stop();
        
        sync.start();
        g.wait_done();
        g.flip();
        g.vsync(); // When this is only a few ms (0-1?) it flickers
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
