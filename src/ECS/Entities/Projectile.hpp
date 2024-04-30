#pragma once
#include "../ECS.hpp"
#include <eigen3/Eigen/Dense>
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"

using vec = Eigen::Vector2f;

class Projectile : public Entity{
public:
    Projectile(Manager& man, std::string name, std::string sprite_name, Sprite::Animation fly, int rng, vec position, float speed, vec velocity);    // Overloading (testing)
    ~Projectile(){};

    int range;                  // How far it the projectile will go before being deleted.
    int distance;               // How far the projectile has travelled
    Sprite::Animation flying_animation; // Animations (TODO: generalize).

    void update() override;
};