#include "workqueue.h"
#include "draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/ioctl.h>

#include "../workqueue/gpu_workqueue_ioctl.h"

static FILE *fp = NULL;

void workqueue_init(){
    if (fp == NULL){
        fp = fopen("/dev/gpu_workqueue", "r+");
        if (fp == NULL){
            errx(1, "Failed to open /dev/gpu_workqueue, is the kernel module inserted?\n");
        }
    }
}

int workqueue_get_num_cores(void){
    return ioctl(fileno(fp), IOCTL_WORKQUEUE_GET_NUM_CORES, NULL);
}

void workqueue_wait_done(void){
    fflush(fp);
    fgetc(fp);
}

static void workqueue_add(int queuenum, struct workorder *o){
    static int dat[8];
    
    dat[0] = (queuenum << 24) | (o->size << 16) | (o->type);
    memcpy(dat+1, o->data, o->size);
    
    fwrite((char *)dat, 1, 32, fp);
}

static struct workorder s;

void workqueue_sof(int queuenum, unsigned int addr){
    s.type = TYPE_SOF;
    s.size = 4;
    unsigned int *data = (unsigned int *)(&s.data);
    data[0] = addr;
    
    workqueue_add(queuenum, &s);
}

void workqueue_eof(int queuenum){
    s.type = TYPE_EOF;
    s.size = 0;
    
    workqueue_add(queuenum, &s);
}

void workqueue_line(int queuenum, int x1, int x2, int y, int col){
    s.type = TYPE_LINE;
    s.size = 8;
    
    short *data = (short *)(&s.data);
    data[0] = x1;
    data[1] = x2;
    data[2] = y;
    data[3] = col;
    
    workqueue_add(queuenum, &s);
}

void workqueue_rect(int queuenum, int x1, int y1, int x2, int y2, int col){
    s.type = TYPE_RECT;
    s.size = 10;
    
    short *data = (short *)(&s.data);
    data[0] = x1;
    data[1] = y1;
    data[2] = x2;
    data[3] = y2;
    data[4] = col;
    
    workqueue_add(queuenum, &s);
}

void workqueue_tri(int queuenum, int x1, int y1, int x2, int y2, int x3, int y3, int col){
    s.type = TYPE_TRI;
    s.size = 14;

   short *data = (short *)(&s.data);
   data[0] = x1;
   data[1] = y1;
   data[2] = x2;
   data[3] = y2;
   data[4] = x3;
   data[5] = y3;
   data[6] = col;

   workqueue_add(queuenum, &s);
}

void workqueue_copy(int queuenum, char *data, char size){
    s.type = TYPE_COPY;
    s.size = size;
    
    memcpy(s.data, data, size);
    
    workqueue_add(queuenum, &s);
}

void workqueue_copy_start(int queuenum){
    s.type = TYPE_COPY_START;
    s.size = 0;
    
    workqueue_add(queuenum, &s);
}
