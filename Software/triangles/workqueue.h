#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#define NUM_QUEUES 4

// Types of work
#define TYPE_SOF    (1)
#define TYPE_EOF    (2)
#define TYPE_LINE   (3)
#define TYPE_RECT   (4)
#define TYPE_TRI    (5)
#define TYPE_COPY   (6)
#define TYPE_COPY_START (7)

struct workorder {
    char type;
    char size;
    char data[16];
};

struct altera_mm_fifo_csr {
    unsigned int fill_level;
    unsigned int i_status;
    unsigned int event;
    unsigned int int_enable;
    unsigned int almostfull;
    unsigned int almostempty;
};

void workqueue_init(int queuenum, void *base, void *csr);

void workqueue_sof(int queuenum, unsigned int addr);
void workqueue_eof(int queuenum);
void workqueue_line(int queuenum, int x1, int x2, int y, int col);
void workqueue_rect(int queuenum, int x1, int y1, int x2, int y2, int col);
void workqueue_tri(int queuenum, int x1, int y1, int x2, int y2, int x3, int y3, int col);
void workqueue_copy(int queuenum, char *data, char size);
void workqueue_copy_start(int queuenum);

#endif /* WORKQUEUE_H */
