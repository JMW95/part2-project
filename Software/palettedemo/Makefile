CXXFLAGS = -O3 -Wall -g -std=c++0x -I../3dlib/ -L../3dlib/
LDLIBS = -l3dlib -lrt

all: software

hardware: main.cpp ../3dlib/lib3dlib.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS) -DHARDWARE_RENDER

software: main.cpp ../3dlib/lib3dlib.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

../3dlib/lib3dlib.a:
	make -C ../3dlib/

clean:
	rm -rf hardware software $(OBJECTS)
