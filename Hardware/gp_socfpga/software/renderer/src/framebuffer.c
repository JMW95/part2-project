#include "framebuffer.h"

static unsigned int bufferaddr = BUFFER1;

void vid_setbuffer(unsigned int addr){
    bufferaddr = addr;
}

// plot a single pixel
void vid_set_pixel(int x, int y, int colour){
    // derive a pointer to the framebuffer described as 4 bit integers
    volatile char *framebuffer = (volatile char *) (bufferaddr);
    
    // make sure we don't go past the edge of the screen
    if ((x<0) || (x>DISPLAY_WIDTH-1))
        return;
    if ((y<0) || (y>DISPLAY_HEIGHT-1))
        return;
    
    int pixelpos = x+y*DISPLAY_WIDTH;
    int bufpos = pixelpos/2;
    
    char val = framebuffer[bufpos];
    if((pixelpos & 1) == 0){ // if x even, use lower 4 bits.
        val = (val & 0xf0) | (colour & 0x0f);
    }else{ // use upper 4 bits for odd x
        val = (val & 0x0f) | ((colour & 0x0f) << 4);
    }
    
    framebuffer[bufpos] = val;
}

// clear the whole screen
void vid_clear(int colour){
    volatile int *framebuffer = (volatile int *) (bufferaddr);
    colour = colour & 0x0f;
    colour = colour | (colour << 4);
    colour = colour | (colour << 8);
    colour = colour | (colour << 16); // 32 bits
    int i = 0;
    int end = (DISPLAY_WIDTH * DISPLAY_HEIGHT)/8;
    while ( i < end )
        framebuffer[i++] = colour;
}

// draw a horizontal line
void vid_fill_line(int left, int right, int top, int colour){
    volatile char *framebuffer = (volatile char *) (bufferaddr);
    int width = right - left;
    colour |= (colour << 4);
    
    int i = (DISPLAY_WIDTH*top + left) / 2; // first bufferpos to fill
    
    if (width > 1 && ((right & 1) != 0)){
        // fill the 'odd half' of the end pixel
        framebuffer[i+(width/2)] = (framebuffer[i+(width/2)] & 0xf0) | (colour & 0x0f);
    }
    
    if ((left & 1) != 0) {
        // fill the 'odd half' of the start pixel
        framebuffer[i] = (framebuffer[i] & 0x0f) | (colour & 0xf0);
        i++;
    }
        
    int endpos = (DISPLAY_WIDTH*top + right)/2;
    int len = endpos - i;
    if(len < 12){ // if less than 3 words, copy bytes
        while (i < endpos)
            framebuffer[i++] = colour;
        
        return;
    }
    
    int c32 = colour | (colour << 8); // extend to 32-bits
    c32 |= (c32 << 16);
    
    unsigned int t;
    
    volatile char *dst = framebuffer + i;
    // Align dst by filling in bytes
    if ((t = (int)dst & 3) != 0){
        t = 4 - t; // number of bytes to fill
        len -= t;
        do {
            *dst++ = colour;
        } while (--t != 0);
    }
    
    // Fill as many full words as possible
    t = len / 4;
    do {
        *(unsigned int *)dst = c32;
        dst += 4;
    } while (--t != 0);
    
    // Fill in any trailing bytes
    t = len & 3;
    if (t != 0){
        do {
            *dst++ = colour;
        } while (--t != 0);
    }
    
}

// fill a certain rectangle
void vid_fill_rect(int left, int top, int right, int bottom, int colour){
    volatile char *framebuffer = (volatile char *) (bufferaddr);
    int width = right - left;
    colour |= (colour << 4);
    int y;
    for (y=top; y<bottom; y++){
        int i = (DISPLAY_WIDTH*y + left) / 2; // first bufferpos to fill
        if (width > 1 && ((right & 1) != 0)){
            // fill the 'odd half' of the end pixel
            framebuffer[i+(width/2)] = (framebuffer[i+(width/2)] & 0xf0) | (colour & 0x0f);
        }
        
        if ((left & 1) != 0) {
            // fill the 'odd half' of the start pixel
            framebuffer[i] = (framebuffer[i] & 0x0f) | (colour & 0xf0);
            i++;
        }
        
        int endpos = (DISPLAY_WIDTH*y + right)/2;
        while (i < endpos)
            framebuffer[i++] = colour;
    }
}
