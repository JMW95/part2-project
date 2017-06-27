#include <iostream>
#include <time.h>

#include <signal.h>
#include <cstring>
#include <atomic>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "GPU.h"
#include "Timer.h"

#include "Demo.h"

#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

std::atomic<bool> quit(false);

unsigned int selected;

std::vector<Demo*> demos;

TTF_Font* font;
SDL_Color textFG = {255, 255, 255};

char *menu_text_bg;

void got_signal(int){
    quit.store(true);
}

char colourtable[][3] = {
    {255,0,0},      // Red
    {0,255,0},      // Green
    {0,0,255},      // Blue
    {255,255,0},    // Yellow
    {255,0,255},    // Magenta
    {0,255,255},    // Cyan
    {127,51,0},     // Brown
    {255,255,255},  // White
};

void draw_arrow(GPU &g, short x, short y, short w, short h, short col){
    Triangle2D t = {{{x,y}, {x,(short)(y+h)}, {(short)(x+w),(short)(y+h/2)}}, {0,0,0}, 0, col};
    g.triangle(t);
}

void _place_text(char *fbuff, const char* txt, int tx, int ty, int fcol, int bcol){
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, txt, textFG);
    
    for(int y=0; y<textSurface->h; y++){
        for(int x=0; x<textSurface->w; x++){
            fbuff[(y+ty)*480 + (x+tx)] = ((char*)textSurface->pixels)[y*textSurface->pitch + x] > 0 ? fcol : bcol;
        }
    }
    
    SDL_FreeSurface(textSurface);
}

char *create_menu(){
    int frontcol = 255;
    int backcol = 1;
    
    char *tmp = new char[480 * 272];
    memset(tmp, backcol, 480*272);
    
    int ty = 5;
    int tx = 25;
    
    for(unsigned int i=0; i<demos.size(); i++){
        _place_text(tmp, demos[i]->name.c_str(), tx, ty, frontcol, backcol);
        
        ty += 20;
    }
    
    return tmp;
}

int main(int argc, char *argv[]){
    
    // Ctrl-C will cause the main loop to exit cleanly
    struct sigaction sa;
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = got_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    
    GPU g;
    Timer calc, draw, frame, sync;
    TTF_Init();
    
    selected = 0;
    
    demos.push_back(new ShapesDemo());
    demos.push_back(new SceneDemo());

    g.set_use_hardware(true);
    
    std::cout << "GPU has " << g.get_num_cores() << " cores." << std::endl;
    
    // Load font
    font = TTF_OpenFont("arial.ttf", 16);
    if(font == NULL){
        std::cout << "Failed to load font :(" << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }
    
    //Initialise to a black screen
    g.sof();
    g.eof();
    
    demos[selected]->switch_to(g);
    
    // Create the main menu
    menu_text_bg = create_menu();

    int i=0;
    while(!quit.load()){
        frame.start();
        calc.start();
        
        g.read_buttons();
        
        // Update menu
        if(g.was_button_just_pressed(GPU::NAV_U)){
            selected = selected > 0 ? selected - 1 : selected;
            demos[selected]->switch_to(g);
        }else if(g.was_button_just_pressed(GPU::NAV_D)){
            selected = selected < demos.size()-1 ? selected + 1 : selected;
            demos[selected]->switch_to(g);
        }
        
        // Update the current demo
        demos[selected]->update(g, i);
        
        calc.stop();
        
        sync.start();
        g.wait_done();
        g.flip();
        g.vsync(); // When this is only a few ms (0-1?) it flickers
        sync.stop();
        
        draw.start();
        
        g.sof();
        
        // Render the demo contents
        demos[selected]->render(g, i);
        
        // Render the text menu
        g.copy(menu_text_bg, 5 + (demos.size() * 20));
        
        int tcol = 1;
        draw_arrow(g, 5, 10 + (selected * 20), 10, 10, (tcol * 32) + 31);
        
        g.eof();
        
        draw.stop();
        
        frame.stop();
        
        /*
        std::cout << "Drew " << renderfaces.size() << " tris\t" << \
            "Calc: " << calc.get_milliseconds() << \
            "ms\tDraw: " << draw.get_milliseconds() << "ms\tTotal: " << \
            (calc.get_milliseconds() + draw.get_milliseconds()) << \
            "ms\tFrame: " << frame.get_milliseconds() << "ms\tFPS: " << \
            floor(1/frame.get_seconds()) << "\tSync: " << \
            sync.get_milliseconds() << "ms" << std::endl;
        //*/
        
        i++;
    }
    
    std::cout << "Quitting!" << std::endl;
    
}
