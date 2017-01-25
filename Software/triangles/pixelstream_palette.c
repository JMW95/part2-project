#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/ioctl.h>

#include "pixelstream_palette.h"
#include "../vsync/pixelstream_palette_ioctl.h"

static FILE *fp;

void palette_set_color(char entrynum, short color){
    int data = (color << 8) | entrynum;
    ioctl(fileno(fp), IOCTL_PALETTE_SET_COLOR, &data);
}

unsigned long pixelstream_get_buffer(void){
    return ioctl(fileno(fp), IOCTL_PIXELSTREAM_GET_BUFFER, NULL);
}

void pixelstream_set_buffer(int bufferaddr){
    ioctl(fileno(fp), IOCTL_PIXELSTREAM_SET_BUFFER, &bufferaddr);
}

void pixelstream_wait_sync(void){
    fgetc(fp); // wait for a vsync
}

void pixelstream_palette_init(void){
    fp = fopen("/dev/pixelstream_vsync", "r");
    if (fp == NULL){
        errx(1, "Failed to open /dev/pixelstream_vsync, is the kernel module inserted?\n");
    }
}

void pixelstream_palette_deinit(void){
    fclose(fp);
}