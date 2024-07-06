#pragma once

#include "headers.h"
#include "structs.h"

void init_imgui(SDL_Window*window,SDL_Renderer*renderer)
{
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();

io.IniFilename="";
io.LogFilename="";

ImGui::StyleColorsDark();

ImGui_ImplSDL2_InitForSDLRenderer(window,renderer);
ImGui_ImplSDLRenderer2_Init(renderer);
}

void imgui_create_frame()
{
ImGui_ImplSDLRenderer2_NewFrame();
ImGui_ImplSDL2_NewFrame();
ImGui::NewFrame();
}

void imgui_draw()
{
ImGui::Render();
ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
