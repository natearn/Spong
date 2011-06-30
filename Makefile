CFLAGS=`sdl-config --cflags` -Wall -Wextra -ansi -pedantic

spong: spong.o main.o
	$(CC) `sdl-config --libs` -o $@ $^ 

tags: spong.h spong.c main.c
	ctags $^

.PHONY: clean

clean:
	$(RM) spong *.o tags
