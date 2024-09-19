#pragma once
#include "SDL.h"
#include "Cooldown.hpp"

struct Hurting : public Cooldown{
    Hurting(
        Uint64 cool_down, Uint64 flicker_timer = 150, 
        std::array<int, 3> sdl_colour_mask = {255, 0 ,0},
        int sdl_alpha_mask = 255
    ): Cooldown(cool_down, flicker_timer, sdl_colour_mask, sdl_alpha_mask){};
    void init() override;
    void custom_update() override;
};