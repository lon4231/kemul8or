#pragma once

#include "headers.h"
#include "structs.h"

void update_screen(SDL_Renderer*renderer)
{SDL_RenderPresent(renderer);}

void clear_screen(SDL_Renderer*renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
SDL_SetRenderDrawColor(renderer,r,g,b,a);
SDL_RenderClear(renderer);
}

SDL_Texture*load_texture(SDL_Renderer*renderer,std::string path)
{return IMG_LoadTexture(renderer,path.c_str());}

void free_texture(SDL_Texture*texture)
{SDL_DestroyTexture(texture);}

TTF_Font*load_font(std::string path,int size)
{return TTF_OpenFont(path.c_str(),size);}

void free_font(TTF_Font*font)
{TTF_CloseFont(font);}

void draw_rect(SDL_Renderer*renderer,SDL_Texture*texture,float x,float y,float w,float h,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
SDL_FRect rect={x,y,w,h};
SDL_SetRenderDrawColor(renderer,r,g,b,a);

if (texture==nullptr)
{SDL_RenderFillRectF(renderer,&rect);}
else
{SDL_RenderCopyF(renderer,texture,nullptr,&rect);}
}

SDL_Texture*text_to_texture(SDL_Renderer* renderer, TTF_Font* font,std::string text,Uint8 r,Uint8 g,Uint8 b,Uint8 a) 
{
SDL_Surface*text_surface=TTF_RenderText_Blended(font,text.c_str(),{r,g,b,a});
SDL_Texture*texture=SDL_CreateTextureFromSurface(renderer,text_surface);
SDL_FreeSurface(text_surface);
return texture;
}

std::array<int,2> get_text_size(TTF_Font*font,std::string text)
{
int w,h;
TTF_SizeText(font,text.c_str(),&w,&h);
return{w,h};
}

void draw_text(SDL_Renderer*renderer,TTF_Font*font,std::string text,float x,float y,float linew,float lineh,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
if(linew!=0 && lineh!=0)
{

linew=window_w/linew;
lineh=window_h/lineh;


std::vector<std::string>splitted;
{
size_t start=0;
size_t end=0;
while((end=text.find("\n",start))!=std::string::npos) 
{splitted.push_back(text.substr(start,end-start));start=end+1;}
splitted.push_back(text.substr(start));
}


int size_last=y;
for (int i=0;i<splitted.size();++i)
{
SDL_Texture*texture=text_to_texture(renderer,font,splitted[i],255,255,255,255);
draw_rect(renderer,texture,x,size_last,linew*splitted[i].size(),lineh,r,g,b,a);
free_texture(texture);

size_last+=lineh;
}


}
}

void set_render_target(SDL_Renderer*renderer,SDL_Texture*texture)
{SDL_SetRenderTarget(renderer,texture);}