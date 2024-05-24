#pragma once
#include "../ECS.hpp"
#include <eigen3/Eigen/Dense>
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"

using vec = Eigen::Vector2f;

/** Projectile Entity.
*/
class Projectile : public Entity{
public:

    /** Constructor
        @param man: The Manager object to associate this projectile to.
        @param name: The name to give the Entity (will also be passed to the Collider component).
        @param sprite_name: The projectile's texture id (stored in Game::assets).
        @param fly: An Animation object with information about the projectile's flying animation (currently only one supported).
        @param mRange: The range of the projectile before it is destroyed.
        @param position: The position the projectile should start at.
        @param speed: The projectile's speed.
        @param velocity: The projectile's velocity vector.
    */
    Projectile(
        Manager& man, std::string name, std::string sprite_name, Sprite::Animation fly, 
        int mRange, vec position, float speed, vec velocity);    // Overloading (testing)
    ~Projectile(){};

    int range;                  ///< How far it the projectile will go before being deleted.
    int distance;               ///< How far the projectile has travelled so far.
    Sprite::Animation flying_animation; ///< Animations (TODO: generalize).

    void update() override;
};