#include "avalon_addr.h"
#include "framebuffer.h"

#define WIDTH   (480)
#define HEIGHT  (272)

// Types of work
#define TYPE_SOF    (1)
#define TYPE_EOF    (2)
#define TYPE_LINE   (3)
#define TYPE_RECT   (4)
#define TYPE_TRI    (5)
#define TYPE_COPY   (6)
#define TYPE_COPY_START (7)

static unsigned int copy_counter = 0;

struct workorder {
    char type;
    char size;
    char data[8];
};

// Number of work orders which can be queued.
// 1024 bytes / sizeof(struct workorder), rounded down to nearest power of 2
#define WORK_QUEUE_SIZE     (128)

struct workqueue {
    short doneptr;
    short readyptr;
    struct workorder orders[WORK_QUEUE_SIZE];
};

/*
 * ARM writes the readyptr to mark the end of valid work orders.
 * We write the doneptr to mark the next piece of work we have to do.
 */
int main(void){
    volatile struct workqueue *queue = (struct workqueue *)(SHARED_BASE);
    while(1){
    // read from queue, perform work orders
    while(queue->readyptr == queue->doneptr); // wait here while empty
    
    for(int i=queue->doneptr; i!=queue->readyptr; i = (i+1) & (WORK_QUEUE_SIZE - 1) ){
        
        // read the work order and do it
        
        volatile struct workorder order = queue->orders[i];
        volatile unsigned short *data = (unsigned short *)(&order.data);
        unsigned short x1,x2,y1,y2,col;
        unsigned int addr;
        switch(order.type){
            case TYPE_SOF:
                // read address of which buffer to use
                addr = *(unsigned int *)(&order.data); // TODO use buffer number instead?
                vid_setbuffer(addr);
                vid_clear(0); //TODO hardware fast-clear?
                break;
            case TYPE_EOF:
                // TODO: End of frame, mark framebuffer as locked
                break;
            case TYPE_LINE:
                // read coords, color, fill pixels
                x1 = data[0];
                x2 = data[1];
                y1 = data[2];
                col = data[3];
                vid_fill_line(x1, x2, y1, col);
                break;
            case TYPE_RECT:
                // fill the rect
                x1 = data[0];
                y1 = data[1];
                x2 = data[2];
                y2 = data[3];
                col = data[4];
                vid_fill_rect(x1, y1, x2, y2, col);
                break;
            case TYPE_TRI:
                // TODO: draw the triangle
                break;
            case TYPE_COPY:
                // Copy bytes to the framebuffer
                vid_copy(copy_counter, (char *)order.data, order.size);
                copy_counter += order.size;
                break;
            case TYPE_COPY_START:
                copy_counter = 0;
                break;
        }
        
        // advance the doneptr
        queue->doneptr = (queue->doneptr + 1) & (WORK_QUEUE_SIZE - 1);
    }
}
}
