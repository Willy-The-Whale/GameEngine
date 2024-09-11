#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_audio.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_opengl3.h>
#include <istream>
#include <vector>
#include <cmath>
#include <gl/GL.h>

#include "Game.hpp"






int main(int argc, char* argv[]) {
    Game* game = Game::GetInstance();

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameCount = 0;
    const int32 velocityIterations = 4;
    const int32 positionIterations = 2;
    const float TimeStep = 1 / 60.0f;

    Uint64 frameStart;
    int startTime = SDL_GetTicks();
    int frameTime;    

    b2World* world = game->init("WIGZEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

    while (game->running()) {

        frameStart = SDL_GetTicks();
        game->handleEvents();        
        game->update();
        game->render();

        world->Step(TimeStep, velocityIterations, positionIterations);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        frameCount++;
        if (SDL_GetTicks() - startTime > 1000) {
            cout << "Average FPS: " << frameCount << endl;
            frameCount = 0;
            startTime += 1000;
        }

    }

    game->clean();


    return 0;
}