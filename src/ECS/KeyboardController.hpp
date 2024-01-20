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
                    transform->set_vy(-1.0);
                    break;
                case SDLK_a:
                    transform->set_vx(-1.0);
                    break;
                case SDLK_s:
                    transform->set_vy(1.0);
                    break;
                case SDLK_d:
                    transform->set_vx(1.0);
                    break;
                default:
                    break;
            }}
            // Check if a key is released:
            if (Game::event.type == SDL_KEYUP){
                switch (Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->set_vy(0.0);
                    break;
                case SDLK_a:
                    transform->set_vx(0.0);
                    break;
                case SDLK_s:
                    transform->set_vy(0.0);
                    break;
                case SDLK_d:
                    transform->set_vx(0.0);
                    break;
                default:
                    break;
            }}
        }
};
