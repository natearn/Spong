#include <stdio.h>
#include <stdlib.h> //for exit()
#include "SDL.h"

int main (int argc, char *argv[]) {
	
	SDL_Surface *screen = NULL;
	SDL_Event event;
	int quit = 0;
	SDL_Rect paddle;
		paddle.x = 0;
		paddle.y = 0;
		paddle.w = 10;
		paddle.h = 80;

	/* initialize required SDL subsystems (TODO: replace EVERYTHING with the specific list) */
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) { //error on init
		fprintf(stderr,"Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	printf("SDL initialize successful\n");
	atexit(SDL_Quit); //shut down SDL on exit

	/* set the video mode. prefer 8-bit, but accept others */
	if(!(screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_ANYFORMAT ))) {
		fprintf(stderr,"Unable to set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	printf("Set 640x480 at %d bits-per-pixel mode\n", screen->format->BitsPerPixel);


	if(SDL_FillRect( screen, &paddle, SDL_MapRGB( screen->format, 3000, 3000, 3000 ))) {
		fprintf(stderr,"fillrect fail\n");
	}
	SDL_Flip( screen );

	/* wait for window x before closing (Primative, needs improvement) */
	while(!quit) {
		while( SDL_PollEvent( &event ) ) {
			if( event.type == SDL_QUIT ) {
				quit = 1;
			}
		}
	}

	/* shutdown all SDL subsytems */
	SDL_Quit();
	printf("SDL shutdown successful\n");

	return 0;
}
