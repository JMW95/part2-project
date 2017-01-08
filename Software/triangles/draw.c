#include "draw.h"
#include "workqueue.h"

#include <stdio.h>
#include <string.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)<(b)?(b):(a))

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
    int queuenum = (top >= 136) ? 1 : 0;
    workqueue_line(queuenum, left, right, top - (queuenum * 136), colour);
#else
    int width = (right - left)+1;
    colour |= (colour << 4);
    
    volatile char *framebuffer = (char *)ftmp;
    
    int i = (DISPLAY_WIDTH*top + left) / 2; // first bufferpos to fill
    
    if ((right & 1) == 0){ // if end even, it's the first 'half' of a pixel
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
    workqueue_tri(0, tmp.points[0].x, tmp.points[0].y, tmp.points[1].x, tmp.points[1].y, tmp.points[2].x, tmp.points[2].y, col);
    workqueue_tri(1, tmp.points[0].x, tmp.points[0].y-136, tmp.points[1].x, tmp.points[1].y-136, tmp.points[2].x, tmp.points[2].y-136, col);    
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
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT/2);
}

void show(){
    int wq = 0;
    for(wq = 0; wq < 2; wq++){
        int nbytes = (DISPLAY_WIDTH*DISPLAY_HEIGHT/2) / 2; // copy half the bytes to each wq
        int i = wq * nbytes; // starting points
        workqueue_copy_start(wq);
        while(nbytes > 0){
            workqueue_copy(wq, ftmp+i, nbytes < 8 ? nbytes : 8);
            nbytes -= 8;
            i += 8;
        }
    }
}

unsigned int vid_getbuffer(){
    return pixelstream[8];
}

void flip(){
#ifndef HARDWARE_RENDER
    show();
#endif
    if(pixelstream[8] == 0x0){
        pixelstream[8] = 0x400000;
    }else{
        pixelstream[8] = 0x0;
    }
}
