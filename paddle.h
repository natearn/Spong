#ifndef _PADDLE_
#define _PADDLE_

#include "SDL.h"

/*
	Paddle: describes a paddle object
*/
struct Paddle {
	SDL_Rect rect;
	enum direction {NO, UP, DOWN, LEFT, RIGHT} movement;
	Uint32 color;
};

/*
	DrawPaddle: draw the paddle on the surface
	MovePaddle: set the direction field
	UpdatePaddle: update the position of the paddle
*/
struct Paddle *MakePaddle( SDL_Rect shape, SDL_PixelFormat *fmt, Uint8 r, Uint8 g, Uint8 b );
void DeletePaddle( struct Paddle *pad );
void DrawPaddle( struct Paddle *pad, SDL_Surface *dst );
void MovePaddle( struct Paddle *pad, enum direction d );
//int UpdatePaddle( struct Paddle *pad, double time, double rate );

#endif
