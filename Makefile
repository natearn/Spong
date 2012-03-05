CFLAGS=`sdl-config --cflags`
#-Wall -Wextra -ansi -pedantic

.PHONY: all clean

all: spong tags

spong: spong.o main.o
	$(CC) -o $@ $^ `sdl-config --libs`

tags: spong.h spong.c main.c
	ctags $^

clean:
	$(RM) spong *.o tags
