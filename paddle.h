#ifndef _PADDLE_
#define _PADDLE_

#include "SDL.h"

/*
	Paddle: describes a paddle object
*/
struct Paddle {
	SDL_Rect rect;
	Uint32 color;
};

/*
	DrawPaddle: draw the paddle (pad) on the destination surface (dst)
*/
struct Paddle *MakePaddle( SDL_Rect shape, SDL_PixelFormat *fmt, Uint8 r, Uint8 g, Uint8 b );
void DeletePaddle( struct Paddle *pad );
void DrawPaddle( struct Paddle *pad, SDL_Surface *dst );

#endif
