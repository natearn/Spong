#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <SDL.h>

/* Spong_Paddle
	- height, width, and position are in pixels
	- motion is in pixels per millisecond
*/
typedef struct
{
	Uint32 lastUpdateTime;
	SDL_Surface *surface;
	SDL_Rect rect;
	int xMotion;
	int yMotion;
} Spong_Paddle;

Spong_Paddle* Spong_MakePaddle( Uint16 height,
                                Uint16 width,
                                Sint16 xPosn,
                                Sint16 yPosn,
                                int xMotion,
                                int yMotion,
                                Uint32 flags,
                                int bitsPerPixel );

/* obj is allowed to be NULL. In that case, this function does nothing */
void Spong_DeletePaddle( Spong_Paddle* obj );

/* update the object's position based on the time difference */
#if 0
void Spong_UpdatePaddle( Spong_Paddle *obj, Uint32 currentTime );
#endif
#endif
