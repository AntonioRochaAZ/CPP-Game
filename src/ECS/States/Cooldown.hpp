#pragma once
#include "SDL2/SDL.h"
#include "../ECS.hpp"

struct Cooldown : public Component{
    
    Uint64 m_cool_down;                     ///< Cool down time in ms.
    Uint64 m_flicker_timer;                 ///< Period of mask flickering.
    Uint64 m_initial_tick;
    std::array<int, 3> m_sdl_colour_mask;   ///< Colour mask to apply to sprite.
    int m_sdl_alpha_mask;                   ///< Alpha mask to apply to sprite.
    bool m_mask = true;                     ///< Whether to apply the mask to the Sprite at a given frame.
    
    Cooldown(
        Uint64 cool_down, Uint64 flicker_timer,
        std::array<int, 3> sdl_colour_mask = {255, 255, 255},
        int sdl_alpha_mask = 255
    );

    void update() override;
    virtual void custom_update(){};
};