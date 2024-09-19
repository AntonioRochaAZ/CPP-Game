#pragma once
#include "SDL.h"
#include "Cooldown.hpp"

struct Shooting : public Cooldown{
    Shooting(
        Uint64 cool_down
    ): Cooldown(cool_down, 1000000, {255, 255, 255}, 255){};
};