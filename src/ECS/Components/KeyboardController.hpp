#pragma once
#include "../../Game.hpp"
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../Entities/Projectile.hpp"

using vec = Eigen::Vector2f;

class KeyboardController : public Component{
    public:

        bool reverse_sprite;

        KeyboardController() :
            reverse_sprite(true){};
        KeyboardController(bool mReverse_sprite) :
            reverse_sprite(mReverse_sprite){};

        bool projectile=false;

        Transform* transform;
        Sprite* sprite;

        void init() override{
            transform = &entity->getComponent<Transform>();
            sprite = &entity->getComponent<Sprite>();
        }

        void update() override;
};
