#include "Cooldown.hpp"
#include "../Components/Health.hpp"

Cooldown::Cooldown(
    Uint64 cool_down, Uint64 flicker_timer, 
    std::array<int, 3> sdl_colour_mask, int sdl_alpha_mask): 
m_cool_down(cool_down), m_flicker_timer(flicker_timer), 
m_sdl_colour_mask(sdl_colour_mask), m_sdl_alpha_mask(sdl_alpha_mask){
    m_initial_tick = SDL_GetTicks64();
}

void Cooldown::update(){
    Uint64 diff = (SDL_GetTicks64() - m_initial_tick);
    if (diff > m_cool_down){    // SDL_GetTicks64() > m_initial_tick + m_cool_down
        destroy();
    }
    m_mask = (bool) (( diff / m_flicker_timer) %  2);
    custom_update();
}