#pragma once
#include "SDL2/SDL.h"
#include <eigen3/Eigen/Dense>
#include <tuple>

using vec = Eigen::Vector2f;

class Collider;
class Collision{
public:
    // Axis-Aligned Bounding Box collision: collision of two rectangles
    /*static bool rect_AABB(const SDL_Rect& RA, const SDL_Rect& RB){
        if (RA.transform->get_x() + RA.w >= RB.transform->get_x() &&
            RB.transform->get_x() + RB.w >= RA.transform->get_x() &&
            RA.transform->get_y() + RA.h >= RB.transform->get_y() &&
            RB.transform->get_y() + RB.h >= CA.transform->get_y()){
            return true;
        }else{
            return false;
        }
    }
    */

    /** Function for checking if two colliders are colliding.
    This function only checks for AABB collision, which considers that both
    objects are rectangles (which is currently the implementation of the 
    Collider component). */
    static bool collider_AABB(const Collider *CA, const Collider *CB);

    /** Function called when certain types of collision happens, inside of the
    Collision::handle_collisions function. Specifically,
    this function is called when an object's position must be set back in order
    to avoid intersecting objects. This can happend between an immovable object
    and a player (or another entity that cannot cross such objects), or between
    two MOVABLE_OBJECTS etc.

    In those cases, to solve the collision, one or both of the objects must be
    moved back in the opposite direction of their velocity, until at least one of
    the collision conditions defined in collider_AABB is no longer met. This is
    done by calculating the amount of time we must rollback so that one of the 
    conditions is no longer met. Than, we can use this time to update the objects'
    positions (r = r_collision - speed * v * calculated time).

    For more thorough information, check source code comments.

    This function returns:
        1. The amount of time we must rollback (float);
        2. An integer indicating the index (starting at zer) of the condition 
           which is not met. This is useful to knowing in which axis we have 
           to reset the player's position.

    Why not pass the Colliders directly? In the case of IMMOVABLE objects, we'll
    pass an artifical value of speed = 0, even if they are moving, so that we can
    update the colliding MOVABLE_OBJECT correctly. Despite being much more verbose,
    this function definition gives us more flexibility.
    */
    static std::tuple<float, int> get_collision_time( 
        float xa,  float xb,  float ya,  float yb, float speed_a, float speed_b,
        float vxa, float vxb, float vya, float vyb, 
        float wa,  float wb,  float ha,  float hb);


    /** As this name suggests, this function handles collisions, and is called inside
    of Game::update for that reason. This docstring should be better developped later.*/
    static void handle_collisions();

};