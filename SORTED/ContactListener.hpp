#pragma once
#include <box2d/box2d.h>
#include "GameObject.hpp"


class ContactListener:public b2ContactListener
{
public:
	GameObject* Player;
	GameObject* Enemy;
	bool EnemyAboutToFall = false;
private: 
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};
