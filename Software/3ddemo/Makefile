CXXFLAGS = -O3 -Wall -g -std=c++0x -I../3dlib/ -L../3dlib/
LDLIBS = -l3dlib -lrt

OBJECTS = main.o

demo: $(OBJECTS) ../3dlib/lib3dlib.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

../3dlib/lib3dlib.a:
	make -C ../3dlib/

clean:
	rm -rf demo $(OBJECTS)