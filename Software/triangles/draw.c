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
    workqueue_line(left, right, top, colour);
#else
    int width = right - left;
    colour |= (colour << 4);
    
    int i = (DISPLAY_WIDTH*top + left) / 2; // first bufferpos to fill
    
    if (width > 1 && ((right & 1) != 0)){
        // fill the 'odd half' of the end pixel
        ftmp[i+(width/2)] = (ftmp[i+(width/2)] & 0xf0) | (colour & 0x0f);
    }
    
    if ((left & 1) != 0) {
        // fill the 'odd half' of the start pixel
        ftmp[i] = (ftmp[i] & 0x0f) | (colour & 0xf0);
        i++;
    }
        
    int endpos = (DISPLAY_WIDTH*top + right)/2;
    int len = endpos - i;
    if(len < 12){ // if less than 3 words, copy bytes
        while (i < endpos)
            ftmp[i++] = colour;
        
        return;
    }
    
    int c32 = colour | (colour << 8); // extend to 32-bits
    c32 |= (c32 << 16);
    
    unsigned int t;
    
    volatile char *dst = ftmp + i;
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
}

void clear(){
    memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT/2);
}

void show(){
    int i = 0;
    int nbytes = DISPLAY_WIDTH*DISPLAY_HEIGHT/2;
    workqueue_copy_start();
    while(nbytes > 0){
        workqueue_copy(ftmp+i, nbytes < 8 ? nbytes : 8);
        nbytes -= 8;
        i += 8;
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
        pixelstream[8] = 0x40000;
    }else{
        pixelstream[8] = 0x0;
    }
}
