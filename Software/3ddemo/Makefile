CXXFLAGS = -O3 -Wall -g -std=c++0x
LDLIBS = -lrt

OBJECTS = main.o Vector3.o Vector4.o Matrix3.o Matrix4.o GPU.o Model.o Timer.o

demo: $(OBJECTS)
	g++ $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: GPU.h Model.h Timer.h
Vector3.o: Vector3.h
Vector4.o: Vector4.h
Matrix3.o: Matrix3.h Vector3.h
Matrix4.o: Matrix4.h Matrix3.h Vector4.h
GPU.o: GPU.h Triangle.h GPU.cpp
Model.o: Model.h
Timer.o: Timer.h

clean:
	rm -f demo $(OBJECTS)