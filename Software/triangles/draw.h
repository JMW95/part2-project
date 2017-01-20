#ifndef DRAW_H
#define DRAW_H


#define PIXEL16(r,g,b) (((r & 0x1F)<<11) | ((g & 0x3F)<<5) | ((b & 0x1F)<<0))
#define PIXEL24(r,g,b) PIXEL16((r>>3), (g>>2), (b>>3))

#define PIXEL_BLACK     PIXEL24(0,0,0)
#define PIXEL_WHITE     PIXEL24(255,255,255)
#define PIXEL_RED       PIXEL24(255,0,0)
#define PIXEL_GREEN     PIXEL24(0,255,0)
#define PIXEL_BLUE      PIXEL24(0,0,255)
#define PIXEL_CYAN      PIXEL24(0,255,255)
#define PIXEL_MAGENTA   PIXEL24(255,0,255)
#define PIXEL_YELLOW    PIXEL24(255,255,0)

#define COLOUR_BLACK    (0)
#define COLOUR_WHITE    (1)
#define COLOUR_RED      (2)
#define COLOUR_GREEN    (3)
#define COLOUR_BLUE     (4)
#define COLOUR_CYAN     (5)
#define COLOUR_MAGENTA  (6)
#define COLOUR_YELLOW   (7)

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 272

char ftmp[DISPLAY_WIDTH * DISPLAY_HEIGHT / 2];

struct point {
    int x;
    int y;
};

struct triangle {
    struct point points[3];
};

void plot(int x, int y, int col);
void drawHorizontalLine(int x1, int x2, int y, int col);
void drawFlatBottomTriangle(struct point *p1, struct point *p2, struct point *p3, int col);
void drawFlatTopTriangle(struct point *p1, struct point *p2, struct point *p3, int col);
void draw(struct triangle *tri, int col);

unsigned int vid_getbuffer(void);
void clear(void);
void flip(void);

#endif /* DRAW_H */