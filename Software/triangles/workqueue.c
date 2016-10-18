#include "workqueue.h"
#include "draw.h"

#include <stdio.h>
#include <string.h>

static void *base;

void workqueue_init(void *baseaddr){
    base = baseaddr;
}

static void workqueue_add(struct workorder *o){
    volatile struct workqueue *q = (struct workqueue *)base;
    
    int ready = q->readyptr;
    int olddoneptr = ((ready+1)&(WORK_QUEUE_SIZE-1));
    while( olddoneptr == q->doneptr){
        // wait here while full
    }
    
    volatile struct workorder *dest = &q->orders[ready];
    memcpy((void *)dest, (void *)o, 4 + o->size);
    
    q->readyptr = (ready+1) & (WORK_QUEUE_SIZE-1); // advance readyptr
}

static struct workorder s;

void workqueue_sof(unsigned int addr){
    s.type = TYPE_SOF;
    s.size = 4;
    unsigned int *data = (unsigned int *)(&s.data);
    data[0] = addr;
    
    workqueue_add(&s);
}

void workqueue_eof(){
    s.type = TYPE_EOF;
    s.size = 0;
    
    workqueue_add(&s);
}

void workqueue_line(int x1, int x2, int y, int col){
    s.type = TYPE_LINE;
    s.size = 8;
    
    short *data = (short *)(&s.data);
    data[0] = x1;
    data[1] = x2;
    data[2] = y;
    data[3] = col;
    
    workqueue_add(&s);
}

void workqueue_rect(int x1, int y1, int x2, int y2, int col){
    s.type = TYPE_RECT;
    s.size = 10;
    
    short *data = (short *)(&s.data);
    data[0] = x1;
    data[1] = y1;
    data[2] = x2;
    data[3] = y2;
    data[4] = col;
    
    workqueue_add(&s);
}