#ifndef _SPONG_H_
#define _SPONG_H

#include <SDL.h>

#ifndef SPONG_FRAME_RATE
#define SPONG_FRAME_RATE 60
#endif 

/* CUSTOM SPONG EVENTS */
#define SPONG_RENDER_EVENT 1

void Spong_Init();
void Spong_CleanUp();
void Spong_EventLoop();
void Spong_Render();

#endif
