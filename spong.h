#ifndef _SPONG_H_
#define _SPONG_H

/* TODO: These macros should eventually become arguments to Spong_Run */
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_DEPTH 32

#ifndef SPONG_REFRESH_RATE
#define SPONG_REFRESH_RATE 60
#endif 


void Spong_Run( /* args */ );

#endif
