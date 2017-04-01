/* Draw some coloured bars, with double-buffering and vsync
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "mem.h"

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
    
    fbuff = hw_base + (( unsigned long)(ALT_FPGASLVS_OFST + FRAMEBUFFER_BASE) & (unsigned long) (HW_REGS_MASK) );
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
    
    long t1, t2, t3;
    struct timespec time1, time2, time3;
    
    int reps;
    for(reps=0; reps<500; reps++){
        clock_gettime(CLOCK_MONOTONIC, &time1);
        
        clear();
        
        int x,y;
        for(y=0;y<DISPLAY_HEIGHT;y++){
            for(x=0;x<DISPLAY_WIDTH;x++){
                if((x+7*reps)%10 == 0){
                    plot(x,y,1);
                }
            }
        }
        
        clock_gettime(CLOCK_MONOTONIC, &time2);
        
        fgetc(fp); // wait for a vsync
        
        flip();
        
        clock_gettime(CLOCK_MONOTONIC, &time3);
    }
    
    t1 = (time1.tv_sec * 1000) + (time1.tv_nsec / 1e06);
    t2 = (time2.tv_sec * 1000) + (time2.tv_nsec / 1e06);
    t3 = (time3.tv_sec * 1000) + (time3.tv_nsec / 1e06);
    
    printf("DRAW:     Start: %lu, end: %lu, elapsed %lu\n", t1, t2, t2-t1);
    printf("SHOW:     Start: %lu, end: %lu, elapsed %lu\n", t2, t3, t3-t2);
    printf("TOTAL:    Start: %lu, end: %lu, elapsed %lu\n", t1, t3, t3-t1);

}
