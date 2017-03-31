SOURCES = triangles.c draw.c workqueue.c pixelstream_palette.c

all: hardware software

hardware: $(SOURCES)
	gcc -o $@ $^ -lrt -O3 -Wall -DHARDWARE_RENDER

software: $(SOURCES)
	gcc -o $@ $^ -lrt -O3 -Wall

clean:
	rm hardware software
