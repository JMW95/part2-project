#ifndef PIXELSTREAM_PALETTE_H
#define PIXELSTREAM_PALETTE_H

void pixelstream_palette_init(void);
void pixelstream_palette_deinit(void);

void palette_set_color(char entrynum, short color);
unsigned long pixelstream_get_buffer(void);
void pixelstream_set_buffer(int buffernum);
void pixelstream_wait_sync(void);

#endif /* PIXELSTREAM_PALETTE_H */