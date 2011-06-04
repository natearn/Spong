#include <SDL.h>
#include "spong.h"

int main( int argc , char *argv[] )
{
	
	Spong_Init();
	Spong_EventLoop();
	Spong_CleanUp();
	(void)argc;
	(void)argv;
	return 0;
}
