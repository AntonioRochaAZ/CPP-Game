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
    static bool collider_AABB(const Collider *CA, const Collider *CB){
        if (CA->transform->get_x() + CA->width  > CB->transform->get_x() &&
            CB->transform->get_x() + CB->width  > CA->transform->get_x() &&
            CA->transform->get_y() + CA->height > CB->transform->get_y() &&
            CB->transform->get_y() + CB->height > CA->transform->get_y()){

            std::cout << CA->get_tag() << " hit: " << CB->get_tag() << "\n";
            return true;
        }else{ return false; }
    }

    // static int collider_AABB(const Collider *CA, const Collider *CB){
    //     int return_val = 1;


    //     if (CA->transform->get_x() + CA->width  > CB->transform->get_x()){return_val *= 2;}
    //     if (CB->transform->get_x() + CB->width  > CA->transform->get_x()){return_val *= 3;}
    //     if (CA->transform->get_y() + CA->height > CB->transform->get_y()){return_val *= 5;}
    //     if (CB->transform->get_y() + CB->height > CA->transform->get_y()){return_val *= 7;}

    //     if (return_val != 1){
    //         std::cout << CA->get_tag() << " hit: " << CB->get_tag() << "\n";
    //         return return_val;
    //     }else{ return 0; }


    // }
    static std::tuple<float, int> get_collision_time( 
        float xa,  float xb,  float ya,  float yb, float speed_a, float speed_b,
        float vxa, float vxb, float vya, float vyb, 
        float wa,  float wb,  float ha,  float hb)
    {
        std::array<float, 4> t;     // Array for storing time values for each condition
        float min_val = 100000;     // Smallest time value.
        int best_idx = -1;
        float dvx = speed_b*vxb - speed_a*vxa;  // Denominators (for checking division by zero)
        float dvy = speed_b*vyb - speed_a*vya; 

        if (dvx == 0 && dvy == 0){ 
            // Object spawned (?) inside another (damn it)
            std::cout << "Error calculating collision: speed difference = 0" << std::endl;
            throw std::runtime_error("Game::update");
        } // Otherwise, if one of the denominators is zero, increate the t value artificially,
        // avoiding division by zero.
        else if (dvx == 0){ dvx = 0.0001; } else if (dvy == 0) { dvy = 0.0001; }

        t[0] = (xa - xb + wa)/(-dvx); // Changed sign of denominator (equates to inverting sign of velocities)
        t[1] = (xb - xa + wb)/(dvx); // Changed sign of denominator (equates to inverting sign of velocities)
        t[2] = (ya - yb + ha)/(-dvy); // Changed sign of denominator (equates to inverting sign of velocities)
        t[3] = (yb - ya + hb)/(dvy); // Changed sign of denominator (equates to inverting sign of velocities)

        for (int idx = 0; idx < 4; idx++){  // Only positive t are valid
            if (t[idx] < min_val && t[idx] > 0){ min_val = t[idx]; best_idx = idx; } }

        if (min_val == 100000 || best_idx == -1){ // Couldn't find a valid t.
            std::cout << "Error calculating collision: min_val and best_idx" << std::endl;
            throw std::runtime_error("Game::update");
        }

        std::cout << "Something" << std::endl;  // Just for debugging.

        return {min_val, best_idx};
    }
};