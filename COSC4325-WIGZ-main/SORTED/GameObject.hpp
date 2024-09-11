#pragma once
#include "Game.hpp"
#include <box2d/box2d.h>
#include <SDL.h>


class GameObject {

public:
	GameObject(const char* textureSheet, int x, int y,b2World* world, int type);
	~GameObject();

	void Update();
	void Health(float damage);
	void Render();
	void attack(bool isGuarding, GameObject*);
	int score=0;
	
	b2Body* body;
	int bodyType; //0 for player, 3 for enemy
	int FootSensor;//1 for player, 4 for enemy
	int handSensor;//2 for player, 5 for enemy
	bool grounded = false;
	bool Guarding = false;
	bool flip = false;
	bool dead = false;
	float health;
	float attackDuration = 0;

	

private:
	int xpos;
	int ypos;

	SDL_Rect src_rect_;
	SDL_Rect dst_rect_;
	
	

	SDL_Texture* objTexture;
	SDL_Texture* AttackTexture;
	SDL_Texture* BlockTexture;
	SDL_Rect srcRect, destRect;
	



};