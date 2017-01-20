/* Generate some triangles and draw them
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "workqueue.h"
#include "pixelstream_palette.h"

#define NUMTRIANGLES 1000
#define NUMREPS 20

struct triangle generateTri(){
    struct triangle tri;
    
    int i;
    for(i=0; i<3; i++){
        tri.points[i].x = rand() % DISPLAY_WIDTH;
        tri.points[i].y = rand() % DISPLAY_HEIGHT;
    }
    
    return tri;
}

int main(){
    
    pixelstream_palette_init();

#ifdef HARDWARE_RENDER
    printf("---------------------------------\n");
    printf("TRIANGLE DEMO - HARDWARE ASSISTED\n");
    printf("---------------------------------\n");
#else
    printf("-----------------------------\n");
    printf("TRIANGLE DEMO - SOFTWARE ONLY\n");
    printf("-----------------------------\n");
#endif
    
    pixelstream_set_buffer(0);
    
    // Set the palette to bright colours
    palette_set_color(0, PIXEL_BLACK);
    palette_set_color(1, PIXEL_WHITE);
    palette_set_color(2, PIXEL_RED);
    palette_set_color(3, PIXEL_GREEN);
    palette_set_color(4, PIXEL_BLUE);
    palette_set_color(5, PIXEL_CYAN);
    palette_set_color(6, PIXEL_MAGENTA);
    palette_set_color(7, PIXEL_YELLOW);
    palette_set_color(8, PIXEL_BLACK);
    palette_set_color(9, PIXEL_WHITE);
    palette_set_color(10, PIXEL_RED);
    palette_set_color(11, PIXEL_GREEN);
    palette_set_color(12, PIXEL_BLUE);
    palette_set_color(13, PIXEL_CYAN);
    palette_set_color(14, PIXEL_MAGENTA);
    palette_set_color(15, PIXEL_YELLOW);
    
    workqueue_init();
    
    long t1,t2;
    struct timespec time1, time2;
    int mintime = 0xffffff;
    int maxtime = 0;
    long sum = 0;
    
    srand(0); // Fixed random seed
    
    int reps;
    for(reps=0; reps<NUMREPS; reps++){
        
        struct triangle tris[NUMTRIANGLES];
        int cols[NUMTRIANGLES];
        
        int i;
        for(i=0; i<NUMTRIANGLES; i++){
            tris[i] = generateTri();
            //cols[i] = (rand() % 7)+1;
            cols[i] = (i % 7)+1;
        }
        
        clock_gettime(CLOCK_MONOTONIC, &time1);
        
        if(pixelstream_get_buffer() == 0x0){
            for(i=0; i<NUM_QUEUES; i++){
                workqueue_sof(i, 1); // START OF FRAME
            }
        }else{
            for(i=0; i<NUM_QUEUES; i++){
                workqueue_sof(i, 0);
            }
        }
#ifndef HARDWARE_RENDER
        clear();
#endif

        for(i=0; i<NUMTRIANGLES; i++){
           draw(&tris[i], cols[i]);
        }
        
        for(i=0; i<NUM_QUEUES; i++){
            workqueue_eof(i); // END OF FRAME
        }
        
        workqueue_wait_done();
        
        clock_gettime(CLOCK_MONOTONIC, &time2);
        
        flip();
        
        pixelstream_wait_sync();
        
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

    pixelstream_palette_deinit();
    
    return 0;
}
