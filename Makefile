CC = gcc -Wall -std=gnu11
CFLAGS = -I/opt/vc/include
LDFLAGS = -L/opt/vc/lib/ -lbcm_host -lEGL -lGLESv2 -lpthread -lrt

draw_nox: draw_nox.o
	$(CC) $(LDFLAGS) -o draw_nox draw_nox.o

draw_nox.o: draw_nox.c shader_loader.h
	$(CC) $(CFLAGS) -o draw_nox.o -c draw_nox.c


.PHONY: clean test

clean:
	rm -f draw_nox draw_nox.o *~

test: draw_nox
	./draw_nox
