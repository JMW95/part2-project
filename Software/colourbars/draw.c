#include "draw.h"

#include <string.h>

void plot(int x, int y, int colour){
    int pixelpos = x+y*DISPLAY_WIDTH;
    
    ftmp[pixelpos] = colour;
}

void clear(){
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT*sizeof(short));
}

void flip(){
    memcpy((void *)fbuff, (void *)ftmp, DISPLAY_WIDTH*DISPLAY_HEIGHT*sizeof(short));
}
