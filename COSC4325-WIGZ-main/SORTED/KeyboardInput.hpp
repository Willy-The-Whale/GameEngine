#pragma once
#include "Game.hpp"
#include "GameObject.hpp"

class KeyboardInput{

public:
    float speed = 1.0f;
    int blockSteps = 0;
    int jumpSteps = 0;
    float vel = 0;
    float velChange = 0;
    float impulse = 0;

	KeyboardInput(GameObject& player, GameObject& enemy) : player(player) , enemy(enemy){}

	void update(SDL_Event& event){

        b2Vec2 velocity = player.body->GetLinearVelocity();
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
            case SDLK_SPACE:
                // Handle jump input
                if (player.grounded)
                {
                    jumpSteps = 2;
                    cout << "Jump." << endl;
                }
                else
                {
                    cout << "Cannot Jump." << endl;
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                // Handle left movement input
                vel = -speed;
                velChange = vel - velocity.x;
                impulse = player.body->GetMass() * velChange / (1 / 60.0f);
                player.body->ApplyForceToCenter(b2Vec2(impulse, 0), true);

                break;
            case SDLK_RIGHT:
            case SDLK_d:
                // Handle right movement input
                vel = speed;
                velChange = vel - velocity.x;
                impulse = player.body->GetMass() * velChange / (1 / 60.0f);
                player.body->ApplyForceToCenter(b2Vec2(impulse, 0), true);
                break;
                // Add other key cases here
            case SDLK_e:
                // Play attack animation and damage enemies in range
                cout << "Attack" << endl;
                player.attack(enemy.Guarding, &enemy);

                break;
            case SDLK_LSHIFT:
                blockSteps = 6;
                cout << "Guarding" << endl;
                break;
            default: player.body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
            case SDLK_RIGHT:
            case SDLK_d:
                player.body->SetLinearVelocity(b2Vec2(0, velocity.y));
                break;
            }
            break;
        }
        if (blockSteps)
        {
            player.Guarding = true;
            blockSteps--;
        }
        else
        {
            player.Guarding = false;
        }
        if (jumpSteps)
        {
            vel = speed;
            velChange = vel - velocity.x;
            impulse = player.body->GetMass() * velChange / (1 / 60.0f);
            player.body->ApplyForceToCenter(b2Vec2(0, -impulse), true);
            jumpSteps--;
        }
    }

private:
    GameObject& player;
    GameObject& enemy;
};