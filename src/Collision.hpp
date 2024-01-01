#pragma once
#include "SDL2/SDL.h"

class Collider;
class Collision{
    public:
        // Axis-Aligned Bounding Box collision: collision of two rectangles
        static bool rect_AABB(const SDL_Rect& RA, const SDL_Rect& RB){
            if (RA.x + RA.w >= RB.x &&
                RB.x + RB.w >= RA.x &&
                RA.y + RA.h >= RB.y &&
                RB.y + RB.h >= RA.y){
                return true;
            }else{
                return false;
            }
        }
        static bool collider_AABB(const Collider& CA, const Collider& CB){
            if(rect_AABB(CA.collider, CB.collider)){
                std::cout << CA.tag << " hit: " << CB.tag << "\n";
                return true;
            }else{
                return false;
            }
        }
};