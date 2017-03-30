#pragma once

#include <fstream>

#include "Triangle.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 272

#include "../vsync/pixelstream_palette_ioctl.h"
#include "../workqueue/gpu_workqueue_ioctl.h"

class GPU{
private:
    int buf;
    unsigned int num_cores;
    FILE* _pixf;
    FILE* _wqf;
    bool use_hardware;
    bool save_output;
    char ftmp[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    short buttons;
    
    void set_buffer(unsigned int addr);
    Triangle2D trisort(const Triangle2D& tri);
    void write_workpacket(int queuenum, int type, char *data, int datalen);
    
    void _draw_horiz_line(int x1, int x2, int top, int colour);
    void _draw_ftt(Point2D *p1, Point2D *p2, Point2D *p3, int col);
    void _draw_fbt(Point2D *p1, Point2D *p2, Point2D *p3, int col);
    
public:
    static const int TEMPERATURE_ALARM = 0;
    static const int DIALL_CLICK = 1;
    static const int DIALR_CLICK = 2;
    static const int NAV_CLICK = 3;
    static const int NAV_D = 4;
    static const int NAV_R = 5;
    static const int NAV_L = 6;
    static const int NAV_U = 7;
    static const int BUTTON_X = 12;
    static const int BUTTON_Y = 13;
    static const int BUTTON_A = 14;
    static const int BUTTON_B = 15;
    GPU();
    ~GPU();
    void flush();
    void flip();
    void wait_done();
    void vsync();
    void set_palette_color(int entrynum, int color);
    void read_buttons();
    bool is_button_pressed(int button);
    unsigned int get_buffer();
    unsigned int get_num_cores();
    void set_use_hardware(bool use);
    void set_save_output(bool save);
    
    void sof();
    void triangle(const Triangle2D& tri);
    void eof();
};
