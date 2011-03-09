#include <stdio.h>
#include <stdlib.h> //for exit()
#include "SDL.h"
#include "paddle.h"

/* Globals */
unsigned redraw_interval = 1000;
const SDL_Rect PADDLE_ONE = { 20, 180, 10, 120 };
const SDL_Rect PADDLE_TWO = { 610, 180, 10, 120 };

int main (int argc, char *argv[]) {

	SDL_Surface *screen = NULL;
	SDL_Event event;

	/* initialize required SDL subsystems (TODO: replace 'EVERYTHING' with the specific modules) */
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) { //error on init
		fprintf(stderr,"Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	atexit(SDL_Quit); //shut down SDL on exit or returning from main

	/* set the video mode. prefer 8-bit, but accept others */
	if(!(screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_ANYFORMAT ))) {
		fprintf(stderr,"Unable to set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	printf("Set 640x480 at %d bits-per-pixel mode\n", screen->format->BitsPerPixel);



	/* make the backgroud */
	SDL_Rect background_rect = { 0, 0, 640, 480 };
	struct Paddle *background = MakePaddle( background_rect, screen->format, 0, 0, 0 );
	/* make the paddles */
	struct Paddle *pad1 = MakePaddle( PADDLE_ONE, screen->format, 255, 255, 255 );
	struct Paddle *pad2 = MakePaddle( PADDLE_TWO, screen->format, 255, 255, 255 );
	/* draw the paddles */
	DrawPaddle( background, screen );
	DrawPaddle( pad1, screen );
	DrawPaddle( pad2, screen );

#if 1
	/* create and initiate timer redraw */
	Uint32 Redraw_callback( Uint32 interval, void *param ) {
		SDL_Event event;
		event.type = SDL_USEREVENT;
		event.user.code = 1; // for now, 1 is the timer redraw event code
		event.user.data1 = NULL;
		event.user.data2 = NULL;
		if( SDL_PushEvent( &event ) ) {
			fprintf(stderr,"Failure to push redraw event to event queue\n");
		}
		return redraw_interval;
	}	
	SDL_TimerID redraw = SDL_AddTimer( redraw_interval, Redraw_callback, NULL );
#endif

	/* refresh screen to show changes to it */
	SDL_Flip( screen );

	/* get next event on the queue, block if no events */
	while( SDL_WaitEvent( &event ) ) {
		if( event.type == SDL_USEREVENT ) {
			if( event.user.code == 1 ) {
				printf("redraw the screen\n");
			}
		} else if( event.type == SDL_QUIT ) {
			break;
		}
	}



	/* clean up paddles */
	DeletePaddle(pad1);
	DeletePaddle(pad2);

	return 0;
}
