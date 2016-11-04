#include "workqueue.h"
#include "draw.h"

#include <stdio.h>
#include <string.h>

static volatile void *bases[8];
static volatile struct altera_mm_fifo_csr *csrs[8];

void workqueue_init(int queuenum, void *baseaddr, void *csraddr){
    bases[queuenum] = baseaddr;
    csrs[queuenum] = csraddr;
}

static void workqueue_add(int queuenum, struct workorder *o){
    while(csrs[queuenum]->fill_level > 100){
        // wait here while almost full
    }
    
    //printf("Fill level: %d\n", csrs[queuenum]->fill_level);
    
    volatile unsigned int *fifowr = (unsigned int *)(bases[queuenum]);
    fifowr[0] = (o->type << 8) | o->size;
    unsigned int *data = (unsigned int *)(&o->data);
    fifowr[0] = data[0];
    if(o->size > 4){
        fifowr[0] = data[1];
    }
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
