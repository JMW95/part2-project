#include "framebuffer.h"
#include "debug.h"

#define abs(x) ((x)<0 ? -(x) : (x))

static unsigned int bufferaddr = BUFFER1;

void vid_setbuffer(unsigned int addr){
    bufferaddr = addr;
}

// clear the whole screen
void vid_clear(int colour){
    volatile int *framebuffer = (volatile int *) (bufferaddr) + (1 << 16);
    colour = colour & 0xff;
    colour = colour | (colour << 8);
    colour = colour | (colour << 16); // 32 bits
    int i = 0;
    int end = (DISPLAY_WIDTH * DISPLAY_HEIGHT)/4; // 4 pixels per int32
    while ( i < end )
        framebuffer[i++] = colour;

}

// draw a horizontal line
void vid_fill_line(int left, int right, int top, int colour){
    volatile int *line_filler = (volatile int *) (bufferaddr) + top;
    unsigned int data;
    // It doesn't matter if left/right need to be swapped - the hardware will do it
	data = (colour << 18) | (right << 9) | (left);
    line_filler[0] = data;
}

// fill a certain rectangle
void vid_fill_rect(int left, int top, int right, int bottom, int colour){
    int y;
    for (y=top; y<bottom; y++){
        vid_fill_line(left, right, y, colour);
    }
}

// fill a triangle  where the points are sorted in ascending y-order
void vid_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int colour){
	//debug_write(3);
	
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

		if(j >= 0){
			if(j >= DISPLAY_HEIGHT) return;
			// Draw this line
			//debug_write(3);
			vid_fill_line(i1, i2, j, colour);
			//debug_write(2);
		}

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

		if(j >= 0){
			if(j >= DISPLAY_HEIGHT) return;
			//debug_write(3);
			vid_fill_line(i2, i3, j, colour);
			//debug_write(2);
		}

		e2 += m2;
		e3 += m3;
	}
}

// Copy some pixel data to the screen
void vid_copy(unsigned int offset, char src[], char count){
    volatile char *framebuffer = (volatile char *) (bufferaddr) + (1 << 18);
    int i=0;
    for(i=0; i<count; i++){
        framebuffer[offset + i] = src[i];
    }
}
