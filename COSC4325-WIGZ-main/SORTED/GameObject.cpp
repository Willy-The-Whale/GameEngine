#include "GameObject.hpp"
#include "TextureManager.hpp"
#include "Game.hpp"
#include "Map.hpp"



GameObject::GameObject(const char* textureSheet, int x, int y, b2World* world, int type) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	AttackTexture = TextureManager::LoadTexture("assets/Effects.png");
	BlockTexture = TextureManager::LoadTexture("assets/Block.png");
	bodyType = type;
	FootSensor = type + 1;
	handSensor = type + 2;
	xpos = x;
	ypos = y;
	health = 100;
	float PhysX = (float)x / 100;
	float PhysY = (float)y / 100;

	//Define the body
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;

	//Set the position of the body in the physics world.
	bdef.position.Set(PhysX+.32f, PhysY+ .32f);

	//Tag the body
	bdef.userData.pointer = bodyType;

	//Create the body
	body = world->CreateBody(&bdef);

	//Define the shape of the body
	b2PolygonShape pshape;
	pshape.SetAsBox(.320f, .320f);

	//Define fixture
	b2FixtureDef fdef;
	fdef.shape = &pshape;
	fdef.density = .8f;
	fdef.restitution = 0.0f;
	fdef.isSensor = false;

	//Attach fixture to body
	body->CreateFixture(&fdef);

	//Sensor Fixture to detect if its grounded.
	pshape.SetAsBox(.32f,.32f, b2Vec2(0, .32f), 0);
	
	fdef.isSensor = true;
	fdef.density = 0.0f;
	b2FixtureUserData f;
	f.pointer = FootSensor;
	fdef.userData = f;

	//Attach sensor to body
	b2Fixture* footSensorFixture = body->CreateFixture(&fdef);
	
}



GameObject::~GameObject()
{
	
}


/*
Update function()-
Gets the game objects coordinates from the physics world, converts them into pixel coordinates, and stores them to be rendered
*/
void GameObject::Update() {
	b2Vec2 position;
	position = body->GetPosition();
	//Convert physics coordinates to pixel coordinates
	position.x -= .32;
	position.x *= 100;

	position.y -= .32;
	position.y *= 100;

	xpos = position.x;
	ypos = position.y;

	
	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	//Where the game object is rendered on screen; x,y of the top left corner, width and height
	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}


/*
Health() function-
Takes in damage then lowers the gameobject's health based on that.
*/
void GameObject::Health(float damage) {
    GameObject::health -= damage;
	if (health<= 0)
	{
		dead = true;
	}

}

/*
attack() function-
First gets positions and angles for attacker and victim.
Then compares positions and angle to check if they are in attack range.
If so, do damage to victim.
*/
void GameObject::attack(bool isGuarding, GameObject* other)

{
	b2Vec2 myPosition = body->GetPosition();
	float myAngle = body->GetAngle();
	float attackRange = 1.0f;
	float damage = 3.0f;
	attackDuration = 10;

	// check if other body is within attack range
	b2Vec2 otherPosition = other->body->GetPosition();
	float distance = b2Distance(myPosition, otherPosition);
	if (distance <= attackRange)
	{
		// check if other body is in front of me
		b2Vec2 direction = b2Vec2(cosf(myAngle), sinf(myAngle));
		b2Vec2 otherDirection = otherPosition - myPosition;
		otherDirection.Normalize();

		//Dot is the angle between the two bodies, use this to ensure attacks occur on sides only.
		float dot = b2Dot(direction, otherDirection);
		//cout << "distance:" << distance << endl;
		//cout << "3rd loop" << endl;
		//cout << "Dot:" << dot << endl;
		if (dot >= 0.7f || dot <= -0.7f)
		{
			//cout << "4th loop" << endl;

			// check if target is guarding
			if (other->Guarding)
			{
				damage /= 2.0f; // reduce damage by half
			}
			// deal damage to target
			other->Health(damage);
			cout << "Damage" << endl;			
		}
		else if (dot > -.1f || dot < .1f)
		{
			//cout << "5th loop" << endl;
			// check if target is guarding
			if (other->Guarding)
			{
				damage /= 2.0f; // reduce damage by half
			}
			// deal damage to target
			other->Health(damage);
			cout << "Damage" << endl;
		}
	}
}






void GameObject::Render() {
	if (flip)
	{
		SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
		if (attackDuration > 0)
		{
			SDL_RenderCopyEx(Game::renderer, AttackTexture, NULL, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
			attackDuration--;
		}
		
	}
	else
	{
		SDL_RenderCopy(Game::renderer, objTexture, NULL, &destRect);
		if (attackDuration > 0)
		{
			SDL_RenderCopy(Game::renderer, AttackTexture, NULL, &destRect);
			attackDuration--;
		}
	}
	if (Guarding)
		{
			SDL_RenderCopy(Game::renderer, BlockTexture, NULL, &destRect);
		}
	
}