#pragma once

#include "headers.h"
#include "structs.h"

_MOUSE_T mouse;
bool running=true;
Uint32 lastTime = SDL_GetTicks();
float delta_time=0;

char keyboard_input=0;

Uint32 window_w=800;
Uint32 window_h=600;

void QUIT_ALL()
{
Mix_Quit();
TTF_Quit();
IMG_Quit();
SDL_Quit();
exit(0);
}

void INIT_ALL()
{
if(SDL_Init(SDL_INIT_EVERYTHING)<0)                      {QUIT_ALL();}
if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG)                 {QUIT_ALL();}
if(TTF_Init() < 0)                                       {QUIT_ALL();}
if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){QUIT_ALL();}
Mix_AllocateChannels(16);
}


void handle_event()
{
SDL_Event event;
Uint32 currentTime = SDL_GetTicks();
delta_time = (currentTime - lastTime) / 1000.0f; 
lastTime = currentTime;
SDL_GetMouseState(&mouse.posx,&mouse.posy);
SDL_GetRelativeMouseState(&mouse.spdx, &mouse.spdy);

keyboard_input='\0';

while (SDL_PollEvent(&event) != 0) 
{
ImGui_ImplSDL2_ProcessEvent(&event);
switch(event.type)
{
case SDL_QUIT:running=false;break;

case SDL_MOUSEBUTTONDOWN:
switch(event.type)
{
case SDL_BUTTON_LEFT:  mouse.clk_l=true;break;
case SDL_BUTTON_MIDDLE:mouse.clk_m=true;break;
case SDL_BUTTON_RIGHT: mouse.clk_r=true;break;
default:break;
}
break;

case SDL_MOUSEBUTTONUP:
switch(event.type)
{
case SDL_BUTTON_LEFT:  mouse.clk_l=false;break;
case SDL_BUTTON_MIDDLE:mouse.clk_m=false;break;
case SDL_BUTTON_RIGHT: mouse.clk_r=false;break;
default:break;
}
break;

case SDL_WINDOWEVENT:
if(event.window.event==SDL_WINDOWEVENT_RESIZED) 
{
window_w=event.window.data1;
window_h=event.window.data2;
}
break;


default:break;
}
}
}
