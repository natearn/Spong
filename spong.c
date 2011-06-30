#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "spong.h"
#include <assert.h>

#define SCREEN_HEIGHT 480

typedef struct
{
	SDL_Surface* surface;
	SDL_Rect position;
	int motion;
	Uint32 reftime;
} DisplayObject;

void Spong_UpdatePosition( DisplayObject* obj )
{
	Uint32 curtime = SDL_GetTicks();
	assert( obj );
	switch(obj->motion)
	{
		case 1:
			obj->position.y -= ( curtime - obj->reftime );
			if( obj->position.y < 0 )
			{
				obj->position.y = 0;
			}
			break;
		case -1:
			obj->position.y += ( curtime - obj->reftime );
			if( obj->position.y + obj->surface->h > SCREEN_HEIGHT )
			{
				obj->position.y = SCREEN_HEIGHT - obj->surface->h;
			}
			break;
		default:
			break;
	}
	obj->reftime = curtime;
}

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

void Spong_Init()
{
	/* TODO: decide if I need to use timer ID */
	SDL_TimerID timerId;
	SDL_Surface *screen = NULL;

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
	if( !(screen = SDL_SetVideoMode( 640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT )))
	{
		fprintf(stderr,"Unable to set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	printf("Set 640x480 at %d bits-per-pixel mode\n", screen->format->BitsPerPixel);

	/* add timer render event */
	if( !(timerId = SDL_AddTimer( 1000 / SPONG_FRAME_RATE , Spong_PushRenderEvent , NULL )) )
	{
		fprintf(stderr,"failure to add timer\n");
		exit(1);
	}

	/* set the window caption */
	SDL_WM_SetCaption( "Spong", NULL );
}

/* not used */
void Spong_CleanUp() {}

/* this will drive the Spong program */
void Spong_Run()
{
	/* resources */
	SDL_Surface *screen = NULL;
	DisplayObject background, paddle;
	SDL_Event event;

	/* initialize screen */
	Spong_Init();
	screen = SDL_GetVideoSurface();

	/* TODO: abstract this out */
	background.surface = SDL_CreateRGBSurface(screen->flags,screen->w,screen->h,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( background.surface, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );
	background.reftime = SDL_GetTicks();
	background.motion=0;
	background.position.x = 0;
	background.position.y = 0;

	paddle.surface = SDL_CreateRGBSurface(screen->flags,20,SCREEN_HEIGHT/6,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( paddle.surface, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );
	paddle.reftime = SDL_GetTicks();
	paddle.motion = 0;
	paddle.position.x = 20;
	paddle.position.y = 20;

	/* event loop */
	while( SDL_WaitEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_KEYDOWN:
				fprintf(stderr,"SDL_KEYDOWN event\n");
				if( event.key.keysym.sym == SDLK_UP )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion += 1;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion -= 1;
				}
				break;
			case SDL_KEYUP:
				if( event.key.keysym.sym == SDLK_UP )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion -= 1;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion += 1;
				}
				break;
			case SDL_USEREVENT:
				switch( event.user.code )
				{
					case SPONG_RENDER_EVENT:
						Spong_UpdatePosition( &paddle );
						SDL_BlitSurface( background.surface, NULL, screen, NULL);
						assert( paddle.surface );
						assert( paddle.surface->w && paddle.surface->h );
						SDL_BlitSurface( paddle.surface, NULL, screen, &paddle.position );
						if( SDL_Flip( screen ) != 0 )
						{
							fprintf(stderr,"FLIP ERROR\n");
							exit(1);
						}
						fprintf(stderr,"%d %d %d\n",paddle.motion, paddle.position.y, paddle.reftime/1000);
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
