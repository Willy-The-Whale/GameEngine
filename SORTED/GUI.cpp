
#include "GUI.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "Game.hpp"


using namespace std;

SDL_Window* Gui::win = nullptr;
SDL_Renderer* Gui::ren = nullptr;

void Gui::Create(SDL_Window* window, SDL_Renderer* renderer) {
	IMGUI_CHECKVERSION();
	/*win = window;
	ren = renderer;*/

	auto* win = Game::GetInstance()->GetSDLwindow();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForSDLRenderer(win,Game::renderer);
	ImGui_ImplSDLRenderer_Init(Game::renderer);
}
void Gui::Shutdown() {
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Gui::HandleSDLEvent(SDL_Event& event) {

	ImGui_ImplSDL2_ProcessEvent(&event);	
}

void Gui::BeginRender() {
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(Game::GetInstance()->GetSDLwindow());
	ImGui::NewFrame();
}
void Gui::EndRender() {
	
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}