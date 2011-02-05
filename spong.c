#include <stdio.h>
#include <stdlib.h> //for exit()
#include "SDL.h"
#include "paddle.h"

/* Constants */
const SDL_Rect PADDLE_ONE = { 20, 180, 10, 120 };
const SDL_Rect PADDLE_TWO = { 610, 180, 10, 120 };

int main (int argc, char *argv[]) {


	SDL_Surface *screen = NULL;
	SDL_Event event;

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

	/* make a paddle and draw it */
	struct Paddle *pad1 = MakePaddle( PADDLE_ONE, screen->format, 255, 255, 255 );
	struct Paddle *pad2 = MakePaddle( PADDLE_TWO, screen->format, 255, 255, 255 );
	DrawPaddle( pad1, screen );
	DrawPaddle( pad2, screen );

#if 0
	/* create and initiate timer flips */
	Uint32 Redraw( Uint32 interval, void *param ) {
	return interval;
	}	
	SDL_TimerID redraw_timer = SDL_AddTimer( 100, Redraw, NULL );
#endif

	/* refresh screen to show changes to it */
	SDL_Flip( screen );

	/* wait for window x before closing (temporary, needs to be replaced with correct procedure) */
	while( SDL_WaitEvent( &event ) ) {
		if( event.type == SDL_QUIT ) {
			break;
		}
		if( event.type == SDL_KEYDOWN ) {
			switch(event.key.keysym.sym) {
				case SDLK_UP:
					if(pad1->rect.y > 10) pad1->rect.y -= 10;
					break;
				case SDLK_DOWN:
					if(pad1->rect.y < 350) pad1->rect.y += 10;
					break;
				default:
					printf("this should never happen\n");
			}
			DrawPaddle( pad1, screen );
			SDL_Flip( screen );
		}
	}

	/* clean up paddles */
	DeletePaddle(pad1);
	DeletePaddle(pad2);

	/* shutdown all SDL subsytems */
	SDL_Quit();
	printf("SDL shutdown successful\n");

	return 0;
}
