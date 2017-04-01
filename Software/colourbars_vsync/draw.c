#include "draw.h"

#include <string.h>

void plot(int x, int y, int colour){
    int pixelpos = x+y*DISPLAY_WIDTH;
    
    ftmp[pixelpos] = colour;
}

void clear(){
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT);
}

void show(unsigned int addr){
    memcpy((void *)addr, (void *)ftmp, DISPLAY_WIDTH*DISPLAY_HEIGHT);
}

void flip(){
    if(pixelstream[8] == 0x0){
        show((unsigned int)fbuff + 0x100000);
        pixelstream[8] = 0x100000;
    }else{
        show((unsigned int)fbuff);
        pixelstream[8] = 0x0;
    }
}
