#ifndef WORKQUEUE_H
#define WORKQUEUE_H

// Types of work
#define TYPE_SOF    (1)
#define TYPE_EOF    (2)
#define TYPE_LINE   (3)
#define TYPE_RECT   (4)
#define TYPE_TRI    (5)

struct workorder {
    char type;
    char size;
    char data[8];
};

// Number of work orders which can be queued.
// 1024 / sizeof(struct workorder), rounded down to nearest power of 2
#define WORK_QUEUE_SIZE     (64)

struct workqueue {
    short doneptr;
    short readyptr;
    struct workorder orders[WORK_QUEUE_SIZE];
};

void workqueue_init(void *base);

void workqueue_sof(unsigned int addr);
void workqueue_eof(void);
void workqueue_line(int x1, int x2, int y, int col);
void workqueue_rect(int x1, int y1, int x2, int y2, int col);

#endif /* WORKQUEUE_H */