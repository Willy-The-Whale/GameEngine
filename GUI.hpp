#pragma once
#include <SDL.h>
#include "imgui.h"
#include "Game.hpp"



class Gui {
public:
    Gui() {};
    ~Gui() {};

    void Create(SDL_Window* window, SDL_Renderer* renderer);
    void Shutdown();

    void HandleSDLEvent(SDL_Event& e);

    void BeginRender();
    void EndRender();

    static SDL_Window* win;
    static SDL_Renderer* ren;
    
};

