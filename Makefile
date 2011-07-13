CFLAGS=`sdl-config --cflags` -Wall -Wextra -ansi -pedantic

.PHONY: all clean

all: spong tags

spong: spong.o main.o
	$(CC) `sdl-config --libs` -o $@ $^ 

tags: spong.h spong.c main.c
	ctags $^

clean:
	$(RM) spong *.o tags
