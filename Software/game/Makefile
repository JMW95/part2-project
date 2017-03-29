CXXFLAGS = -O3 -Wall -g -std=c++0x -I../3dlib/ -L../3dlib/
LDLIBS = -l3dlib -lrt

OBJECTS = main.o Primitives.o Powerup.o Entity.o Teapot.o World.o Tree.o

demo: $(OBJECTS) ../3dlib/lib3dlib.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

../3dlib/lib3dlib.a:
	make -C ../3dlib/

Primitives.o: Primitives.h
Entity.o: Entity.h
Powerup.o: Entity.h
Teapot.o: Entity.h
World.o: World.h
Tree.o: Entity.h

clean:
	rm -rf demo $(OBJECTS)