#include "draw.h"

#include <string.h>

void plot(int x, int y, int colour){
    int pixelpos = x+y*DISPLAY_WIDTH;
    int bufpos = pixelpos/2;
    
    char val = ftmp[bufpos];
    if((pixelpos & 1) == 0){ // if x even, use lower 4 bits.
        val = (val & 0xf0) | (colour & 0x0f);
    }else{ // use upper 4 bits for odd x
        val = (val & 0x0f) | ((colour & 0x0f) << 4);
    }
    
    ftmp[bufpos] = val;
}

void clear(){
    short tmp = palette[0];
    palette[0] = PIXEL_BLACK;
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT/2);
    palette[0] = tmp;
}

void show(unsigned int addr){
    memcpy((void *)addr, (void *)ftmp, DISPLAY_WIDTH*DISPLAY_HEIGHT/2);
}

void flip(){
    if(pixelstream[8] == 0x0){
        show((unsigned int)fbuff + 0x10000);
        pixelstream[8] = 0x40000;
    }else{
        show((unsigned int)fbuff);
        pixelstream[8] = 0x0;
    }
}
