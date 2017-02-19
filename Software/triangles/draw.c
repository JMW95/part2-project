#include "draw.h"
#include "workqueue.h"
#include "pixelstream_palette.h"

#include <stdio.h>
#include <string.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)<(b)?(b):(a))

void plot(int x, int y, int colour){
    int bufpos = x+y*DISPLAY_WIDTH;
    
    ftmp[bufpos] = colour;
}

// Sort the points by y-coordinate so that V1 is the highest.
struct triangle sort(struct triangle *tri){
    struct triangle out;
    struct point tmp;
    int i;
    for(i=0;i<3;i++){
        out.points[i].x = tri->points[i].x;
        out.points[i].y = tri->points[i].y;
    }
    if(out.points[0].y > out.points[2].y){// swap 0 and 2
        tmp.x = out.points[0].x;
        tmp.y = out.points[0].y;
        out.points[0].x = out.points[2].x;
        out.points[0].y = out.points[2].y;
        out.points[2].x = tmp.x;
        out.points[2].y = tmp.y;
    }
    
    if(out.points[0].y > out.points[1].y){// swap 0 and 1
        tmp.x = out.points[0].x;
        tmp.y = out.points[0].y;
        out.points[0].x = out.points[1].x;
        out.points[0].y = out.points[1].y;
        out.points[1].x = tmp.x;
        out.points[1].y = tmp.y;
    }
    
    if(out.points[1].y > out.points[2].y){// swap 1 and 2
        tmp.x = out.points[1].x;
        tmp.y = out.points[1].y;
        out.points[1].x = out.points[2].x;
        out.points[1].y = out.points[2].y;
        out.points[2].x = tmp.x;
        out.points[2].y = tmp.y;
    }
    return out;
}

void drawHorizontalLine(int x1, int x2, int top, int colour){
    int left = min(x1, x2);
    int right = max(x1, x2);
    
#ifdef HARDWARE_RENDER
    int queuenum = top/(DISPLAY_HEIGHT/num_cores);
    workqueue_line(queuenum, left, right, top - (queuenum * (DISPLAY_HEIGHT/num_cores)), colour);
#else
    int width = (right - left)+1;
    
    volatile char *framebuffer = (char *)ftmp;
    
    int i = DISPLAY_WIDTH*top + left; // first bufferpos to fill
        
    int endpos = DISPLAY_WIDTH*top + left + width;
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
    
    int tmp = (t+3) / 4; // t is at least 1
    
    // Use a Duff's Device to optimise this fill loop
    unsigned int *dst32 = (unsigned int *)dst;
    dst32 -= (4 - (t & 3)) & 3; // align to 128-bit boundary.
    switch(t & 3){
        case 0: do{ dst32[0] = c32;
        case 3: dst32[1] = c32;
        case 2: dst32[2] = c32;
        case 1: dst32[3] = c32;
            dst32 += 4;
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
    
#endif
}

void drawFlatBottomTriangle(struct point *p1, struct point *p2, struct point *p3, int col){
    //printf("fbt: (%d,%d), (%d,%d), (%d,%d)\n", p1->x, p1->y, p2->x, p2->y, p3->x, p3->y);
    float invslope1 = (float)(p2->x - p1->x) / (float)(p2->y - p1->y);
    float invslope2 = (float)(p3->x - p1->x) / (float)(p3->y - p1->y);
    
    float currx1 = p1->x;
    float currx2 = p1->x;
    
    int scanlineY;
    for(scanlineY = p1->y; scanlineY <= p2->y; scanlineY++){
        //printf("line y:%d, %d-%d\n", scanlineY, (int)currx1, (int)currx2);
        drawHorizontalLine((int)currx1, (int)currx2, scanlineY, col);
        
        currx1 += invslope1;
        currx2 += invslope2;
    }
}


void drawFlatTopTriangle(struct point *p1, struct point *p2, struct point *p3, int col){
    //printf("ftt: (%d,%d), (%d,%d), (%d,%d)\n", p1->x, p1->y, p2->x, p2->y, p3->x, p3->y);
    float invslope1 = (float)(p3->x - p1->x) / (float)(p3->y - p1->y);
    float invslope2 = (float)(p3->x - p2->x) / (float)(p3->y - p2->y);
    
    float currx1 = p1->x;
    float currx2 = p2->x;
    
    int scanlineY;
    for(scanlineY = p1->y; scanlineY <= p3->y; scanlineY++){
        //printf("line y:%d, %d-%d\n", scanlineY, (int)currx1, (int)currx2);
        drawHorizontalLine((int)currx1, (int)currx2, scanlineY, col);
        
        currx1 += invslope1;
        currx2 += invslope2;
    }
}


void draw(struct triangle *tri, int col){
    struct triangle tmp = sort(tri);
    int i;
    //for(i=0; i<3; i++) printf("point%d: (%d, %d)\n", i, tmp.points[i].x, tmp.points[i].y);
#ifdef HARDWARE_RENDER
    short offset = DISPLAY_HEIGHT / num_cores;
    unsigned int start = tmp.points[0].y / offset; // Which is the first core which touches this triangle
    unsigned int end = tmp.points[2].y / offset; // Which is the last core which touches this triangle
    //if(end >= num_cores){
    //    end = num_cores - 1;
    //}
    tmp.points[0].y -= start * offset;
    tmp.points[1].y -= start * offset;
    tmp.points[2].y -= start * offset;
    for(i=start; i<=end; i++){
        workqueue_tri(i, tmp.points[0].x, tmp.points[0].y, tmp.points[1].x, tmp.points[1].y, tmp.points[2].x, tmp.points[2].y, col);
        tmp.points[0].y -= offset;
        tmp.points[1].y -= offset;
        tmp.points[2].y -= offset;
    }
#else
    // flat bottom
    if (tmp.points[1].y == tmp.points[2].y){
        drawFlatBottomTriangle(&tmp.points[0], &tmp.points[1], &tmp.points[2], col);
    }else if (tmp.points[0].y == tmp.points[1].y){
        drawFlatTopTriangle(&tmp.points[0], &tmp.points[1], &tmp.points[2], col);
    }else{
        //printf("Splitting:\n");
        struct point v4;
        v4.x = (int)(tmp.points[0].x + ((float)(tmp.points[1].y - tmp.points[0].y) / (float)(tmp.points[2].y - tmp.points[0].y)) * (tmp.points[2].x - tmp.points[0].x));
        v4.y = tmp.points[1].y;
        drawFlatBottomTriangle(&tmp.points[0], &tmp.points[1], &v4, col);
        drawFlatTopTriangle(&tmp.points[1], &v4, &tmp.points[2], col);
    }
#endif
}

void clear(){
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT);
}

void show(){
    int wq = 0;
    for(wq = 0; wq < num_cores; wq++){
        int nbytes = (DISPLAY_WIDTH*DISPLAY_HEIGHT) / num_cores;
        int i = wq * nbytes; // starting points
        workqueue_copy_start(wq);
        while(nbytes > 0){
            workqueue_copy(wq, ftmp+i, nbytes < 8 ? nbytes : 8);
            nbytes -= 8;
            i += 8;
        }
    }
}

void flip(){
#ifndef HARDWARE_RENDER
    show();
#endif
    if(pixelstream_get_buffer() == 0x0){
        pixelstream_set_buffer(0x400000);
    }else{
        pixelstream_set_buffer(0);
    }
}
