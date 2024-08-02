#pragma once
#include <eigen3/Eigen/Dense>
#include "../ECS.hpp"


using vec = Eigen::Vector2f;

/** Projectile Entity.
*/
struct Projectile : public Entity{

    int range;                  ///< How far it the projectile will go before being deleted.
    int distance;               ///< How far the projectile has travelled so far.
    
    Projectile(
        Manager& man, std::string name,                 // Entity arguments
        vec position, float speed, vec velocity,        // Transform arguments
        std::string sprite_name, float sprite_scale,    // Sprite arguments
        float damage,                                   // Damage arguments
        int rng, Entity* owner = nullptr                // Finally, projectile arguments.
    );

    void update() override;
};
