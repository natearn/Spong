CFLAGS=`sdl-config --cflags` -Wall -Wextra -std=c99 -pedantic

spong: spong.o paddle.o
	$(CC) `sdl-config --libs` -o $@ $^ 

.PHONY: clean

clean:
	$(RM) spong *.o
