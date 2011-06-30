#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "spong.h"

Uint32 Spong_PushRenderEvent( Uint32 interval, void *param )
{
		SDL_Event event;
		(void)param;
		event.type = SDL_USEREVENT;
		event.user.code = SPONG_RENDER_EVENT;
		event.user.data1 = NULL;
		event.user.data2 = NULL;
		if( SDL_PushEvent( &event ) )
		{
			fprintf(stderr,"Failure to push redraw event to event queue\n");
		}
		return interval;
}

void Spong_Init( SDL_Surface* screen, SDL_Surface* paddle )
{
	SDL_TimerID timerId;

	/* initialize required SDL subsystems (TODO: replace 'EVERYTHING' with the specific modules) */
	/* modules: SDL_INIT_TIMER, ... */
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{ 
		fprintf(stderr,"Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	/* register SDL_Quit at exit */
	if( atexit(SDL_Quit) )
	{
		fprintf(stderr,"Unable to register SDL_Quit atexit\n");
	}

	/* set the video mode. prefer 8-bit, but accept others */
	if(!(screen = SDL_SetVideoMode( 640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT )))
	{
		fprintf(stderr,"Unable to set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	printf("Set 640x480 at %d bits-per-pixel mode\n", screen->format->BitsPerPixel);

	/* add timer render event */
	timerId = SDL_AddTimer( (Uint32)(1000 / SPONG_FRAME_RATE) , Spong_PushRenderEvent , (void*)NULL );
}

/* not used */
void Spong_CleanUp() {}

/* this will drive the Spong program */
void Spong_Run()
{
	/* resources */
	SDL_Surface screen, paddle;
	SDL_Event event;
	int motion=0;

	/* initialize resources */
	Spong_Init( &screen, &paddle );

	/* event loop */
	while( SDL_WaitEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_KEYDOWN:
				fprintf(stderr,"SDL_KEYDOWN event\n");
				if( event.key.keysym.sym == SDLK_UP )
				{
					motion += 1;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					motion -= 1;
				}
				break;
			case SDL_KEYUP:
				if( event.key.keysym.sym == SDLK_UP )
				{
					motion -= 1;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					motion += 1;
				}
				break;
			case SDL_USEREVENT:
				switch( event.user.code )
				{
					case SPONG_RENDER_EVENT:
						SDL_Flip( &screen );
						fprintf(stderr,"%d\n",motion);
						break;
					default:
						fprintf(stderr,"unrecognized user event\n");
				}
				break;
			case SDL_QUIT:
				fprintf(stderr,"SDL_QUIT event\n");
				return;
		}
	}
}
