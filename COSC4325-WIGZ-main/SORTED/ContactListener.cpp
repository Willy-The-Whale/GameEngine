#include "ContactListener.hpp"
#include <box2d/box2d.h>

void ContactListener::BeginContact(b2Contact* contact)
{
	//Get fixtures from the contact/collision
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();
	uintptr_t ptrA;
	uintptr_t ptrB;
	//Get data from the fixtures
	ptrA = a->GetUserData().pointer;
	ptrB = b->GetUserData().pointer;
	//If collision involved the ground sensors, the player/enemy is grounded.
	if (ptrA == 1 )
	{
		Player->grounded = true;
		//cout << "[Contact Listener] Player is grounded." << endl;
	}
	else if (ptrA == 4)
	{
		Enemy->grounded = true;
		//cout << "[Contact Listener] Enemy is grounded." << endl;
	}
	if ( ptrA == 1)
	{
		Player->grounded = true;
		//cout << "[Contact Listener] Player is grounded." << endl;
	}
	else if (ptrA == 4)
	{
		Enemy->grounded = true;
		//cout << "[Contact Listener] Enemy is grounded." << endl;
	}

	
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();
	uintptr_t ptrA;
	uintptr_t ptrB;
	ptrA = a->GetUserData().pointer;
	ptrB = b->GetUserData().pointer;
	//If the collision with the ground sensors ends, the player/enemy is grounded.
	if (ptrA == 1)
	{
		Player->grounded = false;
		//cout << "[Contact Listener] Player is not grounded." << endl;
	}
	else if (ptrA == 4)
	{
		Enemy->grounded = false;
		EnemyAboutToFall = true;
		//cout << "[Contact Listener] Enemy is not grounded." << endl;
	}
	if (ptrA == 1)
	{
		Player->grounded = false;
		
		//cout << "[Contact Listener] Player is not grounded." << endl;
	}
	else if (ptrA == 4)
	{
		Enemy->grounded = false;
		EnemyAboutToFall = true;
		//cout << "[Contact Listener] Enemy is not grounded." << endl;
	}
}