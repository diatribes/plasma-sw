CC = gcc
CFLAGS = -pedantic -Wall -std=c99 -lm `pkg-config --cflags --libs raylib`

CFILES = src/main.c

plasma-sw: $(CFILES) clean
	$(CC) $(CFILES) $(CFLAGS) -o plasma-sw -O3 -DNDEBUG

debug: $(CFILES) clean
	$(CC) $(CFILES) $(CFLAGS) -o plasma-sw -g -DDEBUG

clean:
	rm -f plasma-sw
