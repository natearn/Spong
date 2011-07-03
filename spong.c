#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "spong.h"
#include <assert.h>

#define SPONG_RENDER_EVENT 1

#define PADDLE_WIDTH 20
#define BALL_HEIGHT 20
#define	BALL_WIDTH 20


typedef struct
{
	SDL_Surface* surface;
	SDL_Rect position;
	int motion_y;
	int motion_x;
	Uint32 reftime;
} DisplayObject;

void Spong_InitObject( DisplayObject* obj, int x, int y, int mx, int my )
{
	obj->surface = NULL;
	obj->position.y = y;
	obj->position.x = x;
	obj->motion_x = mx;
	obj->motion_y = my;
	obj->reftime = SDL_GetTicks();
}

void Spong_UpdatePosition( DisplayObject* obj )
{
	Uint32 curtime = SDL_GetTicks();
	assert( obj );
	obj->position.y += obj->motion_y * (int)( curtime - obj->reftime ) / 10;
	if( obj->position.y < 0 ) obj->position.y = 0;
	if( obj->position.y > SCREEN_HEIGHT - obj->surface->h ) obj->position.y = SCREEN_HEIGHT - obj->surface->h;
	obj->position.x += obj->motion_x * (int)( curtime - obj->reftime ) / 10;
	if( obj->position.x < 0 ) obj->position.x = 0;
	if( obj->position.x > SCREEN_WIDTH - obj->surface->w ) obj->position.x = SCREEN_WIDTH - obj->surface->w;
	obj->reftime = curtime;
}

void Spong_Bounce( DisplayObject* ball )
{
	if( ball->position.y <= 0 || ball->position.y >= SCREEN_HEIGHT - ball->surface->h ) ball->motion_y *= -1;
	if( ball->position.x <= 0 || ball->position.x >= SCREEN_WIDTH - ball->surface->w ) ball->motion_x *= -1;
}

int Spong_DrawObjects( DisplayObject* objects[], unsigned num, SDL_Surface* screen )
{
	unsigned idx;
	for( idx=0; idx < num; idx++ )
	{
		if( objects[idx] )
		{
			Spong_UpdatePosition( objects[idx] );
			if( SDL_BlitSurface( objects[idx]->surface, NULL, screen, &objects[idx]->position ) != 0 )
			{
				return -1;
			}
		}
	}
	return 0;
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
	/* TODO: decide if I need to use this timer ID */
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
	if( !(screen = SDL_SetVideoMode( SCREEN_WIDTH,
	                                 SCREEN_HEIGHT,
	                                 SCREEN_DEPTH,
	                                 SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT )))
	{
		fprintf(stderr,"Unable to set video mode: %s\n", SDL_GetError());
		exit(1);
	}

	/* add timer render event */
	if( !(timerId = SDL_AddTimer( 1000 / SPONG_REFRESH_RATE , Spong_PushRenderEvent , NULL )) )
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
	SDL_Event event;
	DisplayObject background, paddle, paddle2, ball;

	/* initialize screen */
	Spong_Init();
	screen = SDL_GetVideoSurface();

	Spong_InitObject( &background, 0, 0, 0, 0 );
	background.surface = SDL_CreateRGBSurface(screen->flags,screen->w,screen->h,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( background.surface, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	Spong_InitObject( &paddle, 20, 20, 0, 0 );
	paddle.surface = SDL_CreateRGBSurface(screen->flags,PADDLE_WIDTH,SCREEN_HEIGHT/6,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( paddle.surface, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );

	Spong_InitObject( &paddle2, SCREEN_WIDTH-20-PADDLE_WIDTH, 20, 0, 0 );
	paddle2.surface = SDL_CreateRGBSurface(screen->flags,PADDLE_WIDTH,SCREEN_HEIGHT/6,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( paddle2.surface, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );

	Spong_InitObject( &ball, (SCREEN_WIDTH/2)-BALL_WIDTH, (SCREEN_HEIGHT/2)-BALL_HEIGHT, 3, 7 );
	ball.surface = SDL_CreateRGBSurface(screen->flags,BALL_WIDTH,BALL_HEIGHT,screen->format->BitsPerPixel,0,0,0,0);
	SDL_FillRect( ball.surface, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );

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
					paddle.motion_y -= 10;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y += 10;
				}
				if( event.key.keysym.sym == SDLK_w )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y -= 10;
				}
				if( event.key.keysym.sym == SDLK_s )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y += 10;
				}
				break;
			case SDL_KEYUP:
				if( event.key.keysym.sym == SDLK_UP )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y += 10;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y -= 10;
				}
				if( event.key.keysym.sym == SDLK_w )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y += 10;
				}
				if( event.key.keysym.sym == SDLK_s )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y -= 10;
				}
				break;
			case SDL_USEREVENT:
				switch( event.user.code )
				{
					case SPONG_RENDER_EVENT:
						Spong_UpdatePosition( &paddle );
						Spong_UpdatePosition( &paddle2 );
						Spong_UpdatePosition( &ball );
						Spong_Bounce( &ball );
						SDL_BlitSurface( background.surface, NULL, screen, &background.position );
						assert( paddle.surface );
						assert( paddle.surface->w && paddle.surface->h );
						SDL_BlitSurface( paddle.surface, NULL, screen, &paddle.position );
						SDL_BlitSurface( paddle2.surface, NULL, screen, &paddle2.position );
						SDL_BlitSurface( ball.surface, NULL, screen, &ball.position );
						if( SDL_Flip( screen ) != 0 )
						{
							fprintf(stderr,"FLIP ERROR\n");
							exit(1);
						}
						fprintf(stderr,"%d %d %d\n",paddle.motion_y, paddle.position.y, paddle.reftime/1000);
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
