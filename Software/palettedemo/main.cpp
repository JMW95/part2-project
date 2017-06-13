#include <iostream>
#include <math.h>
#include <time.h>

#include <signal.h>
#include <cstring>
#include <atomic>

#include "GPU.h"
#include "Util.h"

// 0: Use 32 shades of 8 colours
// 1: Use 3/3/2 8-bit colour
#define USE_332 1


#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

int cmap[][3] = {
    {255,0,0},
    {0,255,0},
    {0,0,255},
    {255,255,0},
    {0,255,255},
    {255,0,255},
    {255,128,0},
    {255,255,255},
};

int main(int argc, char *argv[]){

    GPU g;

    #ifdef HARDWARE_RENDER
    g.set_use_hardware(true);
    g.set_save_output(false);
    // Clear screen
    g.sof();
    g.eof();
    g.flip();
    #else
    g.set_use_hardware(false);
    g.set_save_output(true);
    #endif

    for(int i=0; i<256; i++){
        if(USE_332){
            //                             3bits of red      3 bits of green      2 bits of blue
            g.set_palette_color(i, PIXEL24((i & 0b111) << 5, (i & 0b111000) << 2, (i & 0b11000000)));
        }else{
            // 32 shades of 8 colors
            float re = (float)cmap[i/32][0] * (i%32) / (255*32);
            float gr = (float)cmap[i/32][1] * (i%32) / (255*32);
            float bl = (float)cmap[i/32][2] * (i%32) / (255*32);
            float gamma = 1.0;
            g.set_palette_color(i,
                PIXEL24(
                    (int)(pow(re, 1/gamma) * 255),
                    (int)(pow(gr, 1/gamma) * 255),
                    (int)(pow(bl, 1/gamma) * 255)
                )
            );
        }
    }

    g.sof();

    Triangle2D t {
        { {0,0}, {1,1}, {2,2} },
        {1.0, 1.0, 1.0},
        1.0,
        1
    };

    int i=0;
    int r = 16;
    int c = 16;
    short s = 16;
    for(short x=0; x<r; x++){
        for(short y=0; y<c; y++){
            t.color = i;

            //top left
            t.points[0].x = x*s;
            t.points[0].y = y*s;
            //top right
            t.points[1].x = x*s+s;
            t.points[1].y = y*s;
            //bottom right
            t.points[2].x = x*s+s;
            t.points[2].y = y*s+s;
            g.triangle(t);
            //bottom left
            t.points[1].x = x*s;
            t.points[1].y = y*s+s;
            g.triangle(t);

            i = (i+1) % 256;
        }
    }

    g.eof();
    g.vsync();
    g.flip();

}
