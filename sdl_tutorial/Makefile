CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lSDL2 -lGLEW -lGL

run_shader: run_shader.o
	$(CXX) $(LDFLAGS) -o run_shader run_shader.o
run_shader.o: run_shader.cpp
	$(CXX) $(CXXFLAGS) -o run_shader.o -c run_shader.cpp

.PHONY: clean test

clean:
	rm -f run_shader *.o *~

test: run_shader
	./run_shader


