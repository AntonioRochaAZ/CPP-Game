#pragma once
#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component{
    public:
        Transform* transform;
        void init() override{
            transform = &entity->getComponent<Transform>();
        }
        void update() override{
            // Check if a key is pressed:
            if (Game::event.type == SDL_KEYDOWN){
                switch (Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity[1] = -1.0;
                    break;
                case SDLK_a:
                    transform->velocity[0] = -1.0;
                    break;
                case SDLK_s:
                    transform->velocity[1] = 1.0;
                    break;
                case SDLK_d:
                    transform->velocity[0] = 1.0;
                    break;
                default:
                    break;
            }}
            // Check if a key is released:
            if (Game::event.type == SDL_KEYUP){
                switch (Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity[1] = 0.0;
                    break;
                case SDLK_a:
                    transform->velocity[0] = 0.0;
                    break;
                case SDLK_s:
                    transform->velocity[1] = 0.0;
                    break;
                case SDLK_d:
                    transform->velocity[0] = 0.0;
                    break;
                default:
                    break;
            }}
        }
};
