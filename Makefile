CFLAGS=`sdl-config --cflags` -Wall -Wextra -ansi -pedantic

spong: spong.o main.o paddle.o
	$(CC) `sdl-config --libs` -o $@ $^ 

.PHONY: clean

clean:
	$(RM) spong *.o
