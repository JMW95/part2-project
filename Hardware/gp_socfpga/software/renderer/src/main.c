#include "avalon_addr.h"
#include "framebuffer.h"
#include "debug.h"

// Types of work
#define TYPE_SOF    (1)
#define TYPE_EOF    (2)
#define TYPE_LINE   (3)
#define TYPE_RECT   (4)
#define TYPE_TRI    (5)
#define TYPE_COPY   (6)
#define TYPE_COPY_START (7)

static unsigned int copy_counter = 0;
unsigned int coreid = 0;
unsigned int num_cores = 0;

int DISPLAY_HEIGHT = 0;

volatile unsigned int *fifo = (unsigned int *)(SHARED_BASE);

unsigned char *read_fifo(char numbytes){
    static unsigned int buf[64];
    unsigned int *ptr = buf;
    int numreads = (numbytes+3)/4;
    while(numreads--){
        *ptr++ = fifo[0];
    }
    return (unsigned char *)buf;
}

volatile unsigned char *done = (volatile unsigned char *)DONE_IRQ_BASE;
void write_done_irq(unsigned char val){
    done[0] = val;
}

int main(void){
    num_cores = *(unsigned int *)(NUM_CORES_BASE);
    if(num_cores == 1){
        DISPLAY_HEIGHT = 272;
    }else if(num_cores == 2){
        DISPLAY_HEIGHT = 136;
    }else if(num_cores == 4){
        DISPLAY_HEIGHT = 68;
    }else if(num_cores == 8){
        DISPLAY_HEIGHT = 34;
    }

    coreid = *((unsigned int *)COREID_BASE);
    vid_clear(coreid);
    write_done_irq(0);
    while(1){
        // read the work order and do it
        
        unsigned int val = fifo[0];
        unsigned short type = (val >> 8) & 0xff;
        unsigned short len = (val & 0xff);
        unsigned short *data = (unsigned short *)read_fifo(len);
        
        short x1,x2,x3,y1,y2,y3,col;
        unsigned int addr;
        switch(type){
            case TYPE_SOF:
                write_done_irq(0);
                // read address of which buffer to use
                addr = *(unsigned int *)(data);
                vid_setbuffer(addr == 0 ? BUFFER1 : BUFFER2);
                vid_clear(0); //TODO hardware fast-clear?
                //debug_write(0);
                break;
            case TYPE_EOF:
                write_done_irq(1);
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
                // fill the triangle
                x1 = data[0];
                y1 = data[1];
                x2 = data[2];
                y2 = data[3];
                x3 = data[4];
                y3 = data[5];
                col = data[6];
                //debug_write(2);
                vid_fill_triangle(x1, y1, x2, y2, x3, y3, col);
                //debug_write(0);
                break;
            case TYPE_COPY:
                // Copy bytes to the framebuffer
                vid_copy(copy_counter, (char *)data, len);
                copy_counter += len;
                break;
            case TYPE_COPY_START:
                copy_counter = 0;
                break;
        }
    }
}
