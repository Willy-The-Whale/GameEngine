#pragma once
#include "Game.hpp"
#include <box2d/box2d.h>
#include <box2d/b2_fixture.h>
#include <fstream>
#include <string>
#include <sstream>

class Map {

public:
	Map(b2World* world);
	~Map();

	void LoadMap(int arr[20][25]);
	void DrawMap();
	int bodyType; //-1 for static
	void writeToFile(int level[20][25], const char* levelName);
	void readFromFile( int levelNumber);

	void GuiMap(int arr[20][25]);
	int map[20][25];
private:
	b2World* world;
	SDL_Rect src, dest;

	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* water;
	SDL_Texture* brick;
	SDL_Texture* lava;
	SDL_Texture* wood;
	SDL_Texture* ice;
	SDL_Texture* rock;
	SDL_Texture* marsh;

	
	b2Body* bodies[20][25];
};
