#include "spong.h"
#include <SDL.h>

int main( int argc , char *argv[] )
{
	Spong_Run();
#if 0
	SDL_Surface *screen=NULL, *image=NULL;
	SDL_Init( SDL_INIT_EVERYTHING );
	screen = SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT );
	image = SDL_CreateRGBSurface( screen->flags, screen->w, screen->h, screen->format->BitsPerPixel, 0, 0, 0, 0 );
	SDL_FillRect( image, NULL, SDL_MapRGB( image->format, 0, 255, 0 ) );
	SDL_BlitSurface( image, NULL, screen, NULL );
	SDL_Flip( screen );
	SDL_Delay( 2000 );
	SDL_FreeSurface( image );
	SDL_Quit();
#endif
	(void)argc;
	(void)argv;
	return 0;
}
