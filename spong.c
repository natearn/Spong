#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "spong.h"
#include <assert.h>

#define SPONG_RENDER_EVENT 1

#define PADDLE_SPEED 1000


#define PADDLE_WIDTH 20
#define BALL_HEIGHT 20
#define	BALL_WIDTH 20

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32

#ifndef SPONG_REFRESH_RATE
#define SPONG_REFRESH_RATE 30
#endif 

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
	obj->position.y = y;
	obj->position.x = x;
	obj->motion_x = mx;
	obj->motion_y = my;
	obj->reftime = SDL_GetTicks();
}

/* use the bounding box method of detecting collisions */
int Spong_Collision( DisplayObject* object1, DisplayObject* object2 )
{
	if( object1->position.x > object2->position.x + object2->surface->w - 1 ) return 0;
	if( object1->position.y > object2->position.y + object2->surface->h - 1 ) return 0;
	if( object1->position.x + object1->surface->w - 1 < object2->position.x ) return 0;
	if( object1->position.y + object1->surface->h - 1 < object2->position.y ) return 0;
	return 1;
}

void Spong_UpdatePosition( DisplayObject* obj )
{
	Uint32 curtime = SDL_GetTicks();
	assert( obj );
	obj->position.y += obj->motion_y * (int)( curtime - obj->reftime ) / 1000;
	obj->position.x += obj->motion_x * (int)( curtime - obj->reftime ) / 1000;
	obj->reftime = curtime;
}

void Spong_Score(int player)
{
	static int player1=0;
	static int player2=0;
	switch(player)
	{
		case 1:
			player1++;
			break;
		case 2:
			player2++;
			break;
		default:
			break;
	}
	fprintf(stderr,"Score: %d to %d\n", player1, player2 );
}

void Spong_Bounce( DisplayObject* ball )
{
	int fix;
	if( ball->position.y <= 0 )
	{
		fix = 0 - ball->position.y;
		ball->motion_y *= -1;
		ball->position.y = 0 + fix;
		
	}
	else if( ball->position.y >= SCREEN_HEIGHT - ball->surface->h )
	{
		fix = SCREEN_HEIGHT - ball->surface->h - ball->position.y;
		ball->motion_y *= -1;
		ball->position.y = SCREEN_HEIGHT - ball->surface->h + fix;
	}
	if( ball->position.x <= 0 )
	{
		Spong_Score( 2 );
		Spong_InitObject( ball, (SCREEN_WIDTH/2)-BALL_WIDTH, (SCREEN_HEIGHT/2)-BALL_HEIGHT, 500, 400 );
		fprintf(stderr,"successfully reset ball\n");
		
	}
	else if( ball->position.x >= SCREEN_WIDTH - ball->surface->w )
	{
		Spong_Score( 1 );
		Spong_InitObject( ball, (SCREEN_WIDTH/2)-BALL_WIDTH, (SCREEN_HEIGHT/2)-BALL_HEIGHT, 500, 400 );
		fprintf(stderr,"successfully reset ball\n");
	}
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
	                                 SDL_HWSURFACE | SDL_DOUBLEBUF )))
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

void Spong_BallCollision( DisplayObject* ball, DisplayObject* object )
{
	unsigned iterations;
	for(iterations=0; Spong_Collision( ball, object ); iterations++)
	{
		ball->position.x -= ball->motion_x * 0.01;
		ball->position.y -= ball->motion_y * 0.01;
	}
	if( iterations )
	{
		/* apply ball motion alteration */
		ball->motion_x *= -1;
	}
	for(; iterations > 0; iterations--)
	{
		ball->position.x += ball->motion_x * 0.001;
		ball->position.y += ball->motion_y * 0.001;
	}
	/* reapply iterations */
}

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

	Spong_InitObject( &ball, (SCREEN_WIDTH/2)-BALL_WIDTH, (SCREEN_HEIGHT/2)-BALL_HEIGHT, 500, 400 );
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
					paddle.motion_y -= PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y += PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_w )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y -= PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_s )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y += PADDLE_SPEED;
				}
				break;
			case SDL_KEYUP:
				if( event.key.keysym.sym == SDLK_UP )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y += PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_DOWN )
				{
					Spong_UpdatePosition( &paddle );
					paddle.motion_y -= PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_w )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y += PADDLE_SPEED;
				}
				if( event.key.keysym.sym == SDLK_s )
				{
					Spong_UpdatePosition( &paddle2 );
					paddle2.motion_y -= PADDLE_SPEED;
				}
				break;
			case SDL_USEREVENT:
				switch( event.user.code )
				{
					case SPONG_RENDER_EVENT:
						/* OBJECT UPDATING */
						Spong_UpdatePosition( &paddle );
						Spong_UpdatePosition( &paddle2 );
						Spong_UpdatePosition( &ball );
						Spong_BallCollision( &ball, &paddle );
						Spong_BallCollision( &ball, &paddle2 );
						Spong_Bounce( &ball );
						/* OBJECT DRAWING */
						SDL_BlitSurface( background.surface, NULL, screen, &background.position );
						assert( paddle.surface );
						assert( paddle.surface->w && paddle.surface->h );
						SDL_BlitSurface( paddle.surface, NULL, screen, &paddle.position );
						SDL_BlitSurface( paddle2.surface, NULL, screen, &paddle2.position );
						assert( ball.surface );
						assert( ball.surface->w && ball.surface->h );
						SDL_BlitSurface( ball.surface, NULL, screen, &ball.position );
						if( SDL_Flip( screen ) != 0 )
						{
							fprintf(stderr,"FLIP ERROR\n");
							exit(1);
						}
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

