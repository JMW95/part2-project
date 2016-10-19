/* Generate some triangles and draw them
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "mem.h"
#include "workqueue.h"

struct triangle generateTri(){
    struct triangle tri;
    
    int i;
    for(i=0; i<3; i++){
        tri.points[i].x = rand() % 480;
        tri.points[i].y = rand() % 272;
    }
    
    return tri;
}

int main(){
    void *hw_base = hw_init();
    if(hw_base == NULL){
        return 1;
    }
    
    void *lw_base = lw_init();
    if(lw_base == NULL){
        return 1;
    }
    
    FILE *fp = fopen("/dev/pixelstream_vsync", "r");
    if (fp == NULL){
        printf("Failed to open /dev/pixelstream_vsync, is the kernel module inserted?\n");
        return 1;
    }

#ifdef HARDWARE_RENDER
    printf("---------------------------------\n");
    printf("TRIANGLE DEMO - HARDWARE ASSISTED\n");
    printf("---------------------------------\n");
#else
    printf("-----------------------------\n");
    printf("TRIANGLE DEMO - SOFTWARE ONLY\n");
    printf("-----------------------------\n");
#endif
    
#ifndef HARDWARE_RENDER
	fbuff = hw_base + (( unsigned long)(ALT_FPGASLVS_OFST + FRAMEBUFFER_BASE) & (unsigned long) (HW_REGS_MASK) );
#endif
    pixelstream = lw_base + (( unsigned long)(ALT_LWFPGASLVS_OFST + PIXELSTREAM_BASE) & (unsigned long) (LW_REGS_MASK) );
    palette = lw_base + (( unsigned long)(ALT_LWFPGASLVS_OFST + PALETTE_BASE) & (unsigned long) (LW_REGS_MASK) );
    
    pixelstream[8] = 0x0;
    
    // Set the palette to bright colours
    palette[0] = palette[8] = PIXEL_BLACK;
    palette[1] = palette[9] = PIXEL_WHITE;
    palette[2] = palette[10] = PIXEL_RED;
    palette[3] = palette[11] = PIXEL_GREEN;
    palette[4] = palette[12] = PIXEL_BLUE;
    palette[5] = palette[13] = PIXEL_CYAN;
    palette[6] = palette[14] = PIXEL_MAGENTA;
    palette[7] = palette[15] = PIXEL_YELLOW;
    
    workqueue_init(hw_base + (( unsigned long)(ALT_FPGASLVS_OFST + SHARED_BASE) & (unsigned long) (HW_REGS_MASK) ));
    
    long t1,t2;
    struct timespec time1, time2;
    int mintime = 0xffffff;
    int maxtime = 0;
    long sum = 0;
    
    srand(0); // Fixed random seed
    
    #define NUMTRIANGLES 1000
    #define NUMREPS 20
    
    int reps;
    for(reps=0; reps<NUMREPS; reps++){
        
        struct triangle tris[NUMTRIANGLES];
        int cols[NUMTRIANGLES];
        
        int i;
        for(i=0; i<NUMTRIANGLES; i++){
            tris[i] = generateTri();
            cols[i] = (rand() % 7)+1;
        }
        
        clock_gettime(CLOCK_MONOTONIC, &time1);
        
        if(vid_getbuffer() == 0x0){
            workqueue_sof(0x110000); // START OF FRAME
        }else{
            workqueue_sof(0x100000);
        }
#ifndef HARDWARE_RENDER
        clear();
#endif

        for(i=0; i<NUMTRIANGLES; i++){
           draw(&tris[i], cols[i]);
        }
        
        workqueue_eof(); // END OF FRAME
        
        clock_gettime(CLOCK_MONOTONIC, &time2);
        
        flip();
        
        fgetc(fp); // wait for a vsync
        
        t1 = (time1.tv_sec * 1000) + (time1.tv_nsec / 1e06);
        t2 = (time2.tv_sec * 1000) + (time2.tv_nsec / 1e06);
        
        int elapsed = (t2-t1);
        printf("DRAW:     Elapsed %d\n", elapsed);
        
        if (elapsed < mintime) mintime = elapsed;
        if (elapsed > maxtime) maxtime = elapsed;
        sum += elapsed;
        
    }
    
    printf("--------------------\n");
    printf("TIMES:\nMin: %d\nMax: %d\nAvg: %.4f\n", mintime, maxtime, ((float)sum/(float)(NUMREPS)));
    printf("RATE: %lu/s\n", (1000*NUMTRIANGLES*NUMREPS) / sum);

}
