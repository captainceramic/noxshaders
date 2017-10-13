CC = gcc -Wall -std=gnu11
CFLAGS = -I/opt/vc/include/
LDFLAGS = -L/opt/vc/lib/ -lbcm_host -lbrcmEGL -lbrcmGLESv2 -lpthread -lrt

draw_nox: draw_nox.o shader_loader.o
	$(CC) $(LDFLAGS) -o draw_nox shader_loader.o draw_nox.o

draw_nox.o: draw_nox.c
	$(CC) $(CFLAGS) -o draw_nox.o -c draw_nox.c

shader_loader.o: shader_loader.c
	$(CC) $(CFLAGS) -o shader_loader.o -c shader_loader.c

.PHONY: clean test

clean:
	rm -f draw_nox *.o *~

test: draw_nox
	./draw_nox
