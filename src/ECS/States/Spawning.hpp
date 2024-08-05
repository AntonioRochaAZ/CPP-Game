#pragma once
#include "SDL2/SDL.h"
#include "Cooldown.hpp"

struct Spawning : public Cooldown{
    Spawning(
        Uint64 cool_down, Uint64 flicker_timer = 200, 
        std::array<int, 3> sdl_colour_mask = {128, 128, 128},
        int sdl_alpha_mask = 128
    ): Cooldown(cool_down, flicker_timer, sdl_colour_mask, sdl_alpha_mask){};
    void init() override;
    void custom_update() override;
};