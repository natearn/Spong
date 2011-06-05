#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "spong.h"

static SDL_Surface *Spong_Screen;

/* where can I put this? */
Uint32 Spong_PushRenderEvent( Uint32 interval, void *param )
{
		SDL_Event event;
		(void)param;
		event.type = SDL_USEREVENT;
		event.user.code = SPONG_RENDER_EVENT;
		event.user.data1 = NULL;
		event.user.data2 = NULL;
		if( SDL_PushEvent( &event ) ) {
			fprintf(stderr,"Failure to push redraw event to event queue\n");
		}
		return interval;
}

/* this will be the root of the spong program */
void Spong_EventLoop()
{
	SDL_Event event;
	while( SDL_WaitEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_USEREVENT:
				switch( event.user.code )
				{
					case SPONG_RENDER_EVENT:
						SDL_Flip( Spong_Screen );
						fprintf(stderr,"render the screen\n");
						break;
					default:
						fprintf(stderr,"unrecognized user event\n");
				}
				break;
			case SDL_QUIT:
				fprintf(stderr,"SDL_QUIT event\n");
				return;
			default:
				fprintf(stderr,"unrecognized SDL event\n");
		}
	}
}

void Spong_Init()
{
	SDL_TimerID timerId;
	/* initialize required SDL subsystems (TODO: replace 'EVERYTHING' with the specific modules) */
	/* SDL_INIT_TIMER */
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) { 
		fprintf(stderr,"Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	/* register SDL_Quit at exit */
	if( atexit(SDL_Quit) )
	{
		fprintf(stderr,"Unable to register SDL_Quit atexit\n");
	}

	/* set the video mode. prefer 8-bit, but accept others */
	if(!(Spong_Screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_ANYFORMAT ))) {
		fprintf(stderr,"Unable to set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	printf("Set 640x480 at %d bits-per-pixel mode\n", Spong_Screen->format->BitsPerPixel);

	/* add timer render event */
	timerId = SDL_AddTimer( 1000 / SPONG_FRAME_RATE , Spong_PushRenderEvent , NULL );
}

void Spong_CleanUp()
{
}

