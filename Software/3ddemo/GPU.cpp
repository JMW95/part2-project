#include <sys/ioctl.h>
#include <stdexcept>

#include <cstring>

#include "GPU.h"

#define NUMCORES 8

#define TYPE_SOF 1
#define TYPE_EOF 2
#define TYPE_TRI 5
#define TYPE_COPY 6
#define TYPE_COPY_START 7

void GPU::_draw_horiz_line(int x1, int x2, int top, int colour){
    int left = std::min(x1, x2);
    int right = std::max(x1, x2);
    
    int width = (right - left)+1;
    
    volatile char *framebuffer = (char *)ftmp;
    
    int i = DISPLAY_WIDTH*top + left; // first bufferpos to fill
    
    int endpos = DISPLAY_WIDTH*top + left + width;
    int len = endpos - i;
    
    if(len < 12){ // if less than 3 words, copy bytes
        int rem = endpos - i;
        rem = 11 - rem;
        framebuffer += i;
        switch(rem){
            case 0: framebuffer[10] = colour;
            case 1: framebuffer[9] = colour;
            case 2: framebuffer[8] = colour;
            case 3: framebuffer[7] = colour;
            case 4: framebuffer[6] = colour;
            case 5: framebuffer[5] = colour;
            case 6: framebuffer[4] = colour;
            case 7: framebuffer[3] = colour;
            case 8: framebuffer[2] = colour;
            case 9: framebuffer[1] = colour;
            case 10: framebuffer[0] = colour;
        }
        
        return;
    }
    
    int c32 = colour | (colour << 8); // extend to 32-bits
    c32 |= (c32 << 16);
    
    unsigned int t;
    
    volatile char *dst = framebuffer + i;
    // Align dst by filling in bytes
    if ((t = (int)dst & 3) != 0){
        len = (len + t) - 4;
        switch(t){
            case 1: dst[2] = colour;
            case 2: dst[1] = colour;
            case 3: dst[0] = colour;
        }
        dst += (4-t);
        
    }
    
    // Fill as many full words as possible
    t = len / 4;
    
    int tmp = (t+3) / 4; // t is at least 1
    
    // Use a Duff's Device to optimise this fill loop
    unsigned int *dst32 = (unsigned int *)dst;
    dst32 -= (4 - (t & 3)) & 3; // align to 128-bit boundary.
    switch(t & 3){
        case 0: do{ dst32[0] = c32;
        case 3: dst32[1] = c32;
        case 2: dst32[2] = c32;
        case 1: dst32[3] = c32;
            dst32 += 4;
        }while(--tmp > 0);
    }
    dst = (char *)dst32;
    
    // Fill in any trailing bytes
    t = len & 3;
    if (t != 0){
        switch(3-t){
            case 0: dst[2] = colour;
            case 1: dst[1] = colour;
            case 2: dst[0] = colour;
        }
    }
}

void GPU::_draw_ftt(Point2D *p1, Point2D *p2, Point2D *p3, int col){
    float invslope1 = (float)(p3->x - p1->x) / (float)(p3->y - p1->y);
    float invslope2 = (float)(p3->x - p2->x) / (float)(p3->y - p2->y);
    
    float currx1 = p1->x;
    float currx2 = p2->x;
    
    int scanlineY;
    for(scanlineY = p1->y; scanlineY <= p3->y; scanlineY++){
        _draw_horiz_line((int)currx1, (int)currx2, scanlineY, col);
        
        currx1 += invslope1;
        currx2 += invslope2;
    }
}

void GPU::_draw_fbt(Point2D *p1, Point2D *p2, Point2D *p3, int col){
    float invslope1 = (float)(p2->x - p1->x) / (float)(p2->y - p1->y);
    float invslope2 = (float)(p3->x - p1->x) / (float)(p3->y - p1->y);
    
    float currx1 = p1->x;
    float currx2 = p1->x;
    
    int scanlineY;
    for(scanlineY = p1->y; scanlineY <= p2->y; scanlineY++){
        _draw_horiz_line((int)currx1, (int)currx2, scanlineY, col);
        
        currx1 += invslope1;
        currx2 += invslope2;
    }
}

GPU::GPU(){
    _pixf = fopen("/dev/pixelstream_vsync", "r+");
    _wqf = fopen("/dev/gpu_workqueue", "r+");
    
    if (_pixf == NULL){
        throw std::runtime_error("/dev/pixelstream_vsync not found!");
    }
    
    if(_wqf == NULL){
        throw std::runtime_error("/dev/gpu_workqueue not found!");
    }
    
    use_hardware = false;
    
    buf = 1;
    set_buffer(0);
}

GPU::~GPU(){
    fclose(_pixf);
    fclose(_wqf);
}

void GPU::set_buffer(unsigned int addr){
    ioctl(fileno(_pixf), IOCTL_PIXELSTREAM_SET_BUFFER, &addr);
}

Triangle2D GPU::trisort(const Triangle2D& tri){
    Triangle2D out = tri;
    Point2D tmp;
    if(out.points[0].y > out.points[2].y){// swap 0 and 2
        tmp.x = out.points[0].x;
        tmp.y = out.points[0].y;
        out.points[0].x = out.points[2].x;
        out.points[0].y = out.points[2].y;
        out.points[2].x = tmp.x;
        out.points[2].y = tmp.y;
    }
    
    if(out.points[0].y > out.points[1].y){// swap 0 and 1
        tmp.x = out.points[0].x;
        tmp.y = out.points[0].y;
        out.points[0].x = out.points[1].x;
        out.points[0].y = out.points[1].y;
        out.points[1].x = tmp.x;
        out.points[1].y = tmp.y;
    }
    
    if(out.points[1].y > out.points[2].y){// swap 1 and 2
        tmp.x = out.points[1].x;
        tmp.y = out.points[1].y;
        out.points[1].x = out.points[2].x;
        out.points[1].y = out.points[2].y;
        out.points[2].x = tmp.x;
        out.points[2].y = tmp.y;
    }
    return out;
}

void GPU::write_workpacket(int queuenum, int type, char *packetdata, int datalen){
    char data[32];
    data[0] = type;
    data[1] = 0;
    data[2] = datalen;
    data[3] = queuenum;
    for(int i=0; i<datalen; i++){
        data[4+i] = packetdata[i];
    }
    fwrite(data, 1, 32, _wqf);
}

void GPU::flush(){
    fflush(_wqf);
}

void GPU::flip(){
    if(buf == 0){
        buf = 1;
    }else{
        buf = 0;
    }
    set_buffer(buf == 0 ? 0x400000 : 0);
}

void GPU::wait_done(){
    flush();
    fgetc(_wqf);
}

void GPU::vsync(){
    fgetc(_pixf);
}

void GPU::set_palette_color(int entrynum, int color){
    int data = (color << 8) | entrynum;
    ioctl(fileno(_pixf), IOCTL_PALETTE_SET_COLOR, &data);
}

unsigned int GPU::get_buffer(){
    return ioctl(fileno(_pixf), IOCTL_PIXELSTREAM_GET_BUFFER, NULL);
}

void GPU::sof(){
    for(int i=0; i<NUMCORES; i++){
        write_workpacket(i, TYPE_SOF, (char*)&buf, 4);
    }
    if(!use_hardware){
        memset((void *)ftmp, 0, DISPLAY_WIDTH*DISPLAY_HEIGHT);
    }
}

void GPU::triangle(const Triangle2D& in_tri){
    Triangle2D tri = trisort(in_tri);
    if(use_hardware){
        short data[7];
        data[0] = tri.points[0].x;
        data[1] = tri.points[0].y;
        data[2] = tri.points[1].x;
        data[3] = tri.points[1].y;
        data[4] = tri.points[2].x;
        data[5] = tri.points[2].y;
        data[6] = in_tri.color;
        for(int i=0; i<NUMCORES; i++){
            write_workpacket(i, TYPE_TRI, (char *)data, 14);
            data[1] -= 34;
            data[3] -= 34;
            data[5] -= 34;
        }
    }else{
        if (tri.points[1].y == tri.points[2].y){
            _draw_fbt(&tri.points[0], &tri.points[1], &tri.points[2], tri.color);
        }else if (tri.points[0].y == tri.points[1].y){
            _draw_ftt(&tri.points[0], &tri.points[1], &tri.points[2], tri.color);
        }else{
            Point2D v4;
            v4.x = (int)(tri.points[0].x + ((float)(tri.points[1].y - tri.points[0].y) / (float)(tri.points[2].y - tri.points[0].y)) * (tri.points[2].x - tri.points[0].x));
            v4.y = tri.points[1].y;
            _draw_fbt(&tri.points[0], &tri.points[1], &v4, tri.color);
            _draw_ftt(&tri.points[1], &v4, &tri.points[2], tri.color);
        }
    }
}

void GPU::eof(){
    if(!use_hardware){
        int wq = 0;
        for(wq = 0; wq < NUMCORES; wq++){
            int nbytes = (DISPLAY_WIDTH*DISPLAY_HEIGHT) / NUMCORES;
            int i = wq * nbytes; // starting points
            write_workpacket(wq, TYPE_COPY_START, NULL, 0);
            while(nbytes > 0){
                write_workpacket(wq, TYPE_COPY, ftmp+i, nbytes < 16 ? nbytes : 16);
                nbytes -= 16;
                i += 16;
            }
        }
    }
    for(int i=0; i<NUMCORES; i++){
        write_workpacket(i, TYPE_EOF, NULL, 0);
    }
}

void GPU::set_use_hardware(bool use){
    use_hardware = use;
}