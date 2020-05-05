export PKG_CONFIG_PATH = /opt/vc/lib/pkgconfig

CXX = g++
CXXFLAGS = -Wall -std=c++17 `sdl2-config --cflags` `pkg-config brcmglesv2 --cflags`
LDFLAGS = `sdl2-config --libs` `pkg-config brcmglesv2 --libs` 

run_shader: run_shader.o
	$(CXX) $(LDFLAGS) -o run_shader run_shader.o
run_shader.o: run_shader.cpp
	$(CXX) $(CXXFLAGS) -o run_shader.o -c run_shader.cpp

.PHONY: clean test

clean:
	rm -f run_shader *.o *~

test: run_shader
	./run_shader


