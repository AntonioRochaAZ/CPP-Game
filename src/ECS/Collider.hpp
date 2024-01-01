#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Components.hpp"

class Collider : public Component{

public:
    // "Problem": we consider our objects as rectangles for the
    // moment.
    // "AABB" Collision (episode 11 of tutorial).
    SDL_Rect collider;
    std::string tag;    // Will be useful to define what to do


    Collider(){};
    Collider(std::string user_tag){tag = user_tag;};
    Transform* transform;

    SDL_Rect& rect(){return collider;}
    void init() override {
        if (!entity->has_component<Transform>()){
            entity->addComponent<Transform>();
        }
        transform = &entity->getComponent<Transform>();

        Game::collider_vector.push_back(this);
    }

    void update() override {
        collider.x = transform->position[0];
        collider.y = transform->position[1];
        collider.w = transform->width;
        collider.h = transform->height;
    }

};