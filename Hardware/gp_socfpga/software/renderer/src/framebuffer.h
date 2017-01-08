#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "avalon_addr.h"

#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

void vid_set_pixel(int x, int y, int colour);

void vid_fill_line(int left, int right, int top, int colour);

void vid_fill_rect(int left, int top, int right, int bottom, int colour);

void vid_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int colour);

void vid_clear(int colour);

void vid_setbuffer(unsigned int addr);

void vid_copy(unsigned int offset, char src[], char count);

#endif
