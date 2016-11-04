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

struct altera_mm_fifo_csr {
    unsigned int fill_level;
    unsigned int i_status;
    unsigned int event;
    unsigned int int_enable;
    unsigned int almostfull;
    unsigned int almostempty;
};

volatile unsigned int *fifo = (unsigned int *)(SHARED_BASE);
volatile struct altera_mm_fifo_csr *fifo_csr = (struct altera_mm_fifo_csr *)(SHARED_BASE+0x100);

unsigned char *read_fifo(char numbytes){
    static unsigned char buf[256];
    unsigned int *ptr = (unsigned int *)(buf);
    int numreads = (numbytes+3)/4;
    while(numreads--){
        *ptr++ = fifo[0];
    }
    return buf;
}

int main(void){
    while(1){
        // read the work order and do it
        
        unsigned int val = fifo[0];
        unsigned short type = (val >> 8) & 0xff;
        unsigned short len = (val & 0xff);
        unsigned short *data = (unsigned short *)read_fifo(len);
        
        unsigned short x1,x2,y1,y2,col;
        unsigned int addr;
        switch(type){
            case TYPE_SOF:
                // read address of which buffer to use
                addr = *(unsigned int *)(data); // TODO use buffer number instead?
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
                vid_copy(copy_counter, (char *)data, len);
                copy_counter += len;
                break;
            case TYPE_COPY_START:
                copy_counter = 0;
                break;
        }
    }
}
