#pragma once
#include "SDL2/SDL.h"
#include "../ECS.hpp"

struct Shooting : public Component{
    
    Uint64 m_cool_down;
    Uint64 m_initial_tick;
    
    Shooting(Uint64 cool_down): m_cool_down(cool_down){ m_initial_tick = SDL_GetTicks64(); };
    void update() override { 
        if (SDL_GetTicks64() > m_initial_tick + m_cool_down){ 
            destroy();
        } 
    };
};