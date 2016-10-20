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
    int width = (right - left) + 1; // number of pixels to fill
    colour |= (colour << 4);
    
    int i = (DISPLAY_WIDTH*top + left) / 2; // first bufferpos to fill
    
    if ((right & 1) == 0){ // if end even (and end != start), it's the first 'half' of a pixel
        framebuffer[i+(width/2)] = (framebuffer[i+(width/2)] & 0xf0) | (colour & 0x0f);
    }
    
    if ((left & 1) != 0) { // if start odd, its the second 'half' of a pixel
        framebuffer[i] = (framebuffer[i] & 0x0f) | (colour & 0xf0);
        i++;
    }
        
    int endpos = (DISPLAY_WIDTH*top + left + width)/2;
    int len = endpos - i;
    if(len < 12){ // if less than 3 words, copy bytes
        int rem = endpos - i;
        rem = 11 - rem;
        framebuffer += i;
        switch(rem){
            case 0: framebuffer[10] = colour;
            case 1: framebuffer[9] = colour;
            case 2: framebuffer[8] = colour;
            case 3: framebuffer[7] = colour;
            case 4: framebuffer[6] = colour;
            case 5: framebuffer[5] = colour;
            case 6: framebuffer[4] = colour;
            case 7: framebuffer[3] = colour;
            case 8: framebuffer[2] = colour;
            case 9: framebuffer[1] = colour;
            case 10: framebuffer[0] = colour;
        }
        
        return;
    }
    
    int c32 = colour | (colour << 8); // extend to 32-bits
    c32 |= (c32 << 16);
    
    unsigned int t;
    
    volatile char *dst = framebuffer + i;
    // Align dst by filling in bytes
    if ((t = (int)dst & 3) != 0){
        len = (len + t) - 4;
        switch(t){
            case 1: dst[2] = colour;
            case 2: dst[1] = colour;
            case 3: dst[0] = colour;
        }
        dst += (4-t);
    }
    
    // Fill as many full words as possible
    t = len / 4;
    
    // Use a Duff's Device to unroll the copying loop
    int tmp = (t+7) / 8; // t is at least 1
    unsigned int *dst32 = (unsigned int *)dst;
    dst32 -= (8 - (t & 7)) & 7; // align to 128-bit boundary.
    switch(t & 7){
        case 0: do{ dst32[0] = c32;
        case 7: dst32[1] = c32;
        case 6: dst32[2] = c32;
        case 5: dst32[3] = c32;
        case 4: dst32[4] = c32;
        case 3: dst32[5] = c32;
        case 2: dst32[6] = c32;
        case 1: dst32[7] = c32;
            dst32 += 8;
        }while(--tmp > 0);
    }
    dst = (char *)dst32;
    
    // Fill in any trailing bytes
    t = len & 3;
    if (t != 0){
        switch(3-t){
            case 0: dst[2] = colour;
            case 1: dst[1] = colour;
            case 2: dst[0] = colour;
        }
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

// Copy some pixel data to the screen
void vid_copy(unsigned int offset, char src[], char count){
    volatile char *framebuffer = (volatile char *) (bufferaddr);
    int i=0;
    for(i=0; i<count; i++){
        framebuffer[offset + i] = src[i];
    }
}
