#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "../Components.hpp"

/** Options for collision handling */
enum collision_handle : std::size_t{
    IMMOVABLE_ON_COLLISION,       // Cannot move the object.
    MOVABLE_OBJECT,             // The object can move by itself.
    PUSH_ON_COLLISION,        // The object can be pushed.
    DESTROY_ON_COLLISION,    // Destroy the object when any collision happens (e.g. a projectile that hits a wall)
};

/** Options for handling collisions against projectiles.
Currently unused. Could eventually add the possibility to reflect projectiles. */
enum projectile_handle : std::size_t{
    INDESTRUCTABLE_ON_PROJECTILE,       // Projectiles won't affect it
    DESTROY_ON_PROJECTILE,              // Object is destroyed when a projectile hits it
    DAMAGE_ON_PROJECTILE                // Projectile damages it (currently unused, wil be useful when HP is implemented).
};

/** Component used for collision detection. Currently only supports AABB 
    collistion (intersecting rectangles). */
class Collider : public Component{
private:
    std::string tag;        
        /**< This tag could eventually be useful for deciding whether to take 
        the collision into account. */

public:
    bool dynamic_shape;
        /**< DEPRECATED, NOW IN SPRITE COMPONENT (set_collider). 
        Whether the width and height will be updated dynamically 
        according to the Sprite's animation information. */
    
    std::size_t handling_option;    
        ///< Information about if the object can move/be moved or not. It
        ///< should be an entry of the collision_handle enum.

    std::size_t projectile_handling_option;    
        ///< Information about if the object should be destroyed or damaged when
        ///< hit by a projectile.

    int width, height;  ///< Width and Height of the rectangular collider.
    Transform* transform;   ///< A pointer to the transform of the entity.
    // TODO: "Problem": we consider our objects as rectangles for the
    // moment.
    // "AABB" Collision (episode 11 of tutorial).

    // Constructors and Destructors: ----------------------------------------------------------
    Collider(){};
    Collider(std::string user_tag, bool mDynamic_shape, std::size_t mHandling) : 
        tag(user_tag), dynamic_shape(mDynamic_shape), handling_option(mHandling),
        projectile_handling_option(INDESTRUCTABLE_ON_PROJECTILE) {};
    Collider(std::string user_tag) : tag(user_tag), dynamic_shape(false), handling_option(IMMOVABLE_ON_COLLISION),
        projectile_handling_option(INDESTRUCTABLE_ON_PROJECTILE) {};
    Collider(std::string user_tag, int w, int h) : 
        tag(user_tag), dynamic_shape(false), handling_option(IMMOVABLE_ON_COLLISION), 
        projectile_handling_option(INDESTRUCTABLE_ON_PROJECTILE), width(w), height(h){};
    Collider(std::string user_tag, int w, int h, std::size_t mHandling) :
        tag(user_tag), dynamic_shape(false), handling_option(mHandling), 
        projectile_handling_option(INDESTRUCTABLE_ON_PROJECTILE), width(w), height(h){};
    Collider(std::string user_tag, std::size_t mHandling, std::size_t mProjHandling):
        tag(user_tag), dynamic_shape(false), handling_option(mHandling), projectile_handling_option(mProjHandling){};
    ~Collider();    /// Remove collider from Game collider vector.

    // Base class methods: ----------------------------------------------------
    void init() override;
    //void update() override;

    // Getting transform: -----------------------------------------------------
    void get_components();   
        ///< For getting transform when the definition of the collider has preceeded it.
    void get_shape();   
        ///< For getting shape from the Sprite component.
    void set_proj_handling(std::size_t mProjHandling){projectile_handling_option = mProjHandling;};
        ///< Setter function for projectile_handling_option.
    void set_col_handling(std::size_t mHandling){handling_option = mHandling;};
        ///< Setter function for handling_option.
    
    // Collider dimensions: ---------------------------------------------------
    void set_width(int w){ width = w; }
    void set_height(int h){ height = h; }

    // Get tag:
    std::string get_tag() const { return tag; }
};