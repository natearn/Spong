#include "paddle.h"
#include <SDL.h>

Spong_Paddle* Spong_MakePaddle( Uint16 height,
                                Uint16 width,
                                Sint16 xPosn,
                                Sint16 yPosn,
                                int xMotion,
                                int yMotion,
                                Uint32 flags,
                                int bitsPerPixel )
{
	Spong_Paddle *ret = NULL;
	if ( !(ret = malloc(sizeof(*ret))) ) {
		goto memerr;
	}

	if ( !(ret->surface  = SDL_CreateRGBSurface(flags,width,height,bitsPerPixel,0,0,0,0)) ) {
		free(ret);
		goto memerr;
	}

	ret->lastUpdateTime = SDL_GetTicks();
	ret->rect.h         = height;
	ret->rect.w         = width;
	ret->rect.x         = xPosn;
	ret->rect.y         = yPosn;
	ret->xMotion        = xMotion;
	ret->yMotion        = yMotion;

	if( SDL_FillRect( ret->surface , NULL , SDL_MapRGB( ret->surface->format , 255 , 255 , 255 ) ) ) {
		fprintf(stderr,"ERROR:Spong_MakePaddle: failed to fill object surface\n");
		Spong_DeletePaddle( ret );
		ret = NULL;
	}
	return ret;

memerr:
	fprintf(stderr,"ERROR:Spong_MakePaddle: out of memory\n");
	return NULL;
}

void Spong_DeletePaddle( Spong_Paddle* obj )
{
	if ( obj ) {
		if ( obj->surface ) {
			SDL_FreeSurface( obj->surface );
		}
		free( obj );
	}
}
