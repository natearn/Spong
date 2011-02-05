#include "paddle.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct Paddle *MakePaddle( SDL_Rect shape, SDL_PixelFormat *fmt, Uint8 r, Uint8 g, Uint8 b ) {
	assert(fmt);
	struct Paddle *ret = malloc(sizeof(*ret));
	ret->rect = shape;
	ret->color = SDL_MapRGB( fmt, r, g, b );
	ret->movement = NO;
	return ret;
}

void DeletePaddle( struct Paddle *pad ) {
	assert(pad);
	free(pad);
}

void DrawPaddle( struct Paddle *pad, SDL_Surface *dst ) {
	assert(pad);
	assert(dst);
	/* draw the rectangle on the screen with a white(ish) fill */
	if( SDL_FillRect( dst, &pad->rect, pad->color ) ) {
		fprintf(stderr,"DrawPaddle: FillRect error\n");
	}

}

void MovePaddle( struct Paddle *pad, enum direction d ) {
	assert(pad);
	pad->movement = d;
}
