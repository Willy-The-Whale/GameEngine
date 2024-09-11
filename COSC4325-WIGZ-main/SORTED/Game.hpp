#pragma once
#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <SDL_image.h>
#include <box2d/box2d.h>

using namespace std;

class Game {
public:
    static Game* GetInstance() {
        return mInstance = (mInstance != nullptr) ? mInstance : new Game();
    }
    ~Game();

    b2World* init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    b2Vec2 convertCoordinates(float x, float y);

    void enemyAI();
    

    SDL_Window* GetSDLwindow() { return window; }
    static SDL_Renderer *renderer;
    static SDL_Event event;

    bool running() { return isRunning; }

    void renderHealthBar();
    void renderScoreSystem();
    void CreateCeiling();
    void DeathCheck();
    void showGUIsystem();

private:
    Game();
    static Game* mInstance;
    bool isGuarding = false;
    bool isRunning = false;
    int cnt = 0;
    SDL_Window* window;    
    float maxHealth;



};




#endif