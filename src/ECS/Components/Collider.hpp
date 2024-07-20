#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "../Components.hpp"

/** 
    Options for collision handling:
    - IMMOVABLE_ON_COLLISION: Cannot move the object.
    - MOVABLE_OBJECT: The object can move by itself.
    - PUSH_ON_COLLISION: The object can be pushed.
    - PHASE_ON_COLLISION: Collision is identified but it is not moved nor does it stops other objects from moving.
    - DESTROY_ON_COLLISION: Destroy the object when any collision happpens (e.g. a projectile that hits a wall).
*/
enum collision_handle : std::size_t{
    IMMOVABLE_ON_COLLISION,     // Cannot move the object.
    MOVABLE_OBJECT,             // The object can move by itself.
    PUSH_ON_COLLISION,          // The object can be pushed.
    PHASE_ON_COLLISION,         // Collision is identified but it is not moved nor does it stops other objects from moving.
    DESTROY_ON_COLLISION,       // Destroy the object when any collision happens (e.g. a projectile that hits a wall)
};

/** Options for handling collisions against projectiles.
Currently unused. Could eventually add the possibility to reflect projectiles. 

Actually, this probably won't be used, this information will probably be integrated
into collision_handle.
*/
enum projectile_handle : std::size_t{
    INDESTRUCTABLE_ON_PROJECTILE,       // Projectiles won't affect it
    DESTROY_ON_PROJECTILE,              // Object is destroyed when a projectile hits it
    DAMAGE_ON_PROJECTILE                // Projectile damages it (currently unused, wil be useful when HP is implemented).
};

/** Component used for collision detection. Currently only supports AABB 
    collistion (intersecting rectangles). */
struct Collider : public Component{
    std::string tag;        
        /**< This tag could eventually be useful for deciding whether to take 
        the collision into account. */

    bool dynamic_shape;
        /**< This is only used for debugging. The actual handling is done in
        the \ref ::Sprite component with the set_collider attribute. To set
        it on or off, use the enable_dynamic_shape and disable_dynamic_shape 
        functions. They'll ensure that dynamic_shape and the Sprite's set_collider
        members are in sync.
        */

    std::size_t handling_option;    
        ///< Information about if the object can move/be moved or not. It
        ///< should be an entry of the collision_handle enum.

    std::size_t projectile_handling_option;    
        ///< Information about if the object should be destroyed or damaged when
        ///< hit by a projectile.

    int width, height;  ///< Width and Height of the rectangular collider.

    // Constructors and Destructors: ----------------------------------------------------------
    Collider(){};

    Collider(std::string user_tag, std::size_t mHandling, int w = 0, int h = 0) :
        tag(user_tag), dynamic_shape(false), handling_option(mHandling), 
        projectile_handling_option(INDESTRUCTABLE_ON_PROJECTILE), width(w), height(h){};

    ~Collider();    /// Remove collider from Game collider vector.

    // Base class methods: ----------------------------------------------------
    void init() override;
    //void update() override;
    
    // Getting transform: -----------------------------------------------------
    void enable_dynamic_shape(){ 
        dynamic_shape = true;
        entity->getComponent<Sprite>().set_collider = true;
        get_shape();
    };
    void disable_dynamic_shape(){
        dynamic_shape = false;
        entity->getComponent<Sprite>().set_collider = false;
    }
    void get_shape();   ///< For getting shape from the Sprite component.
};