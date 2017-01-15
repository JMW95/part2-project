#include "framebuffer.h"
#include "debug.h"

#define abs(x) ((x)<0 ? -(x) : (x))

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
    if (top < 0 || top >= DISPLAY_HEIGHT) return;
    int swap;
    if (left > right){
		swap = left;
		left = right;
		right = swap;
	}
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

// fill a triangle  where the points are sorted in ascending y-order
void vid_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int colour){
	debug_write(3);
	
	// First draw the top half, from y = y1 to y2
	int i1 = x1;
	int i2 = x1;

	// Line 1, from point 1 to point 2
	int dx1 = x2 - x1;
	int dy1 = y2 - y1;
	int m1 = abs(dx1);
	int iinc1 = -1;
	int e1 = 0;

	if (dx1 > 0){
		iinc1 = 1;
		e1 = -1;
	}

	// Line 2, from point 1 to point 3
	int dx2 = x3 - x1;
	int dy2 = y3 - y1;
	int m2 = abs(dx2);
	int iinc2 = -1;
	int e2 = 0;

	if (dx2 > 0){
		iinc2 = 1;
		e2 = -1;
	}

	// Scan down the y-axis, advancing each instance of Bresenham's algorithm to the new endpoint
	int j;
	for(j=y1; j<y2; j++){
		while(e1 >= 0){
			i1 += iinc1;
			e1 -= dy1;
		}

		while(e2 >= 0){
			i2 += iinc2;
			e2 -= dy2;
		}

		debug_write(3);
		// Draw this line
		vid_fill_line(i1, i2, j, colour);
		debug_write(2);

		e1 += m1;
		e2 += m2;
	}

	// Now draw the bottom half by continuing the line from point 1 to 3, and adding line 3 from point 2 to 3
	// Line 3, from point 2 to point 3
	int i3 = x2;
	int dx3 = x3 - x2;
	int dy3 = y3 - y2;
	int m3 = abs(dx3);
	int iinc3 = -1;
	int e3 = 0;

	if (dx3 > 0){
		iinc3 = 1;
		e3 = -1;
	}

	for(j=y2; j<y3; j++){
		while (e2 >= 0){
			i2 += iinc2;
			e2 -= dy2;
		}

		while (e3 >= 0){
			i3 += iinc3;
			e3 -= dy3;
		}

		debug_write(3);
		vid_fill_line(i2, i3, j, colour);
		debug_write(2);

		e2 += m2;
		e3 += m3;
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
