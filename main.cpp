#include "include.h"




void imgui_ui()
{


ImGui::Begin("emulator debug menu!");

if(ImGui::BeginTabBar("tabs"))
{

if(ImGui::BeginTabItem("terminal"))
{
ImGui::InputInt("char count",&char_count);
ImGui::InputInt("line count",&line_count);
ImGui::InputText("font name",font_name,128);
ImGui::InputInt("font size",&font_size);
if(ImGui::Button("reload font")==true){free_font(font);font=load_font(font_name,font_size);}
ImGui::EndTabItem();
}



if(ImGui::BeginTabItem("simulator"))
{
ImGui::Text("instruction: %s",current_instruction.c_str());
ImGui::InputFloat("hertz",&sim_clock_speed,0.1,1);
ImGui::Text("paused: %s",sim_running?"False":"True");
ImGui::Text("tick: %d",sim_tick);
ImGui::Text("counter: %d",counter);


if(ImGui::Button(sim_running?"sim unpause":"sim pause",{85,20})){sim_running=!sim_running;}
ImGui::SameLine(100,2);
if(ImGui::Button("sim step")){sim_step=true;}
ImGui::SameLine();
if(ImGui::Button("reset")){sim_tick=0;counter=0;for (Uint8&chr:registers){chr=0;}}
ImGui::SameLine();
if(ImGui::Button("triger interrupt")){interrupt=true;}

ImGui::EndTabItem();
}

if(ImGui::BeginTabItem("memory"))
{

ImGui::InputText("file path",memory_file_path,128);

if(ImGui::Button("load hex"))
{
for (Uint8&chr:memory){chr=0;}
parse_hex(memory.data(),read_file(memory_file_path));
}
ImGui::SameLine();
if(ImGui::Button("load bin"))
{
for (Uint8&chr:memory){chr=0;}
std::string file=read_file(memory_file_path);
int i=0;
for (char chr: file)
{
memory[i%memory.size()]=chr;
i++;
}

}

ImGui::Separator();

for (int i=0;i<memory.size();++i)
{
std::array<float,4>color={1,1,1,1};

if(i>=0&&i<=0x1fff)     {color[0]=0;color[1]=1;color[2]=0;}
if(i>0x1fff&&i<0x4000)  {color[0]=1;color[1]=0;color[2]=0;}
if(i>=0x4000&&i<=0x7FFF){color[0]=0;color[1]=0;color[2]=1;}
if(i==0x8000)           {color[0]=1;color[1]=0;color[2]=1;}
if(i==8001)             {color[0]=1;color[1]=1;color[2]=0;}
if(i>0x8002&&i<=0x8004) {color[0]=0;color[1]=1;color[2]=1;}
if(memory[i]==0)        {color[3]=0.5;}

if(i==counter%memory.size()){color[0]=1;color[1]=1;color[2]=1;}

if(i%16==0)
{
ImGui::Text("0x%04x | ",i);
ImGui::SameLine();
}

ImGui::TextColored({color[0],color[1],color[2],color[3]},"%02X",memory[i]);
if(!(i%16 == 15)){ImGui::SameLine();}
}

ImGui::EndTabItem();
}

if(ImGui::BeginTabItem("registers"))
{

for (Uint8 reg:registers)
{
std::array<float,4>color={1,1,1,1};
if(reg==0){color[3]=0.5;}

ImGui::TextColored({color[0],color[1],color[2],color[3]},"%02x",(int)reg);
}


ImGui::EndTabItem();
}

ImGui::EndTabBar();
}


ImGui::End();
}




int main(int argc,char*argv[])
{
INIT_ALL();
SDL_Window*window=SDL_CreateWindow("hola?",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_RESIZABLE);
SDL_Renderer*renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

init_imgui(window,renderer);

font=load_font("font.ttf",64);




std::thread worker=std::thread(sim_worker);

while (running)
{
handle_event();

imgui_create_frame();

clear_screen(renderer,0,0,0,255);

imgui_ui();

imgui_draw();
update_screen(renderer);
}

worker.detach();

return 0;
}