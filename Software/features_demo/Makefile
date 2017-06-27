CXXFLAGS = -O3 -Wall -g -std=c++0x -I../3dlib/ -L../3dlib/
LDLIBS = -l3dlib -lrt -lSDL -lSDL_ttf

OBJECTS = main.o Primitives.o ShapesDemo.o SceneDemo.o

demo: $(OBJECTS) ../3dlib/lib3dlib.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

../3dlib/lib3dlib.a:
	make -C ../3dlib/

clean:
	rm -rf demo $(OBJECTS)