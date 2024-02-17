#pragma once
#include "SDL2/SDL.h"

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
        static bool collider_AABB(const Collider& CA, const Collider& CB){
            if (CA.transform->get_x() + CA.width >= CB.transform->get_x() &&
                CB.transform->get_x() + CB.width >= CA.transform->get_x() &&
                CA.transform->get_y() + CA.height >= CB.transform->get_y() &&
                CB.transform->get_y() + CB.height >= CA.transform->get_y()){
/*
                std::cout << CA.transform->get_x() << " " << CA.width << " " << CA.transform->get_y() << " " << CA.height << std::endl;
                std::cout << CB.transform->get_x() << " " << CB.width << " " << CB.transform->get_y() << " " << CB.height << std::endl;
                throw std::runtime_error("Debugging");*/
                std::cout << CA.get_tag() << " hit: " << CB.get_tag() << "\n";

                return true;
            }else{
                return false;
            }
        }
};