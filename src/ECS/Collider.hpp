#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Components.hpp"

class Collider : public Component{

private:
    std::string tag;        // Will be useful to define what to do

public:
    int width, height;
    Transform* transform;
    // TODO: "Problem": we consider our objects as rectangles for the
    // moment.
    // "AABB" Collision (episode 11 of tutorial).

    // Constructors: ----------------------------------------------------------
    Collider(){};
    Collider(std::string user_tag){tag = user_tag;};
    Collider(std::string user_tag, int w, int h){
        tag = user_tag;
        width = w;
        height = h;
    };

    // Base class methods: ----------------------------------------------------
    void init() override;
    //void update() override;
    
    // Collider dimensions: ---------------------------------------------------
    void set_width(int w){ width = w;}
    void set_height(int h){ height = h;}

    // Get tag:
    std::string get_tag() const {return tag;}
};