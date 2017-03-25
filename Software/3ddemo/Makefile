CXXFLAGS = -O3 -Wall -g -std=c++0x -I../3dlib/ -L../3dlib/
LDLIBS = -l3dlib -lrt

OBJECTS = main.o

demo: $(OBJECTS)
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf demo $(OBJECTS)