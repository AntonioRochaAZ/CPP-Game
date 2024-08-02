#pragma once
#include "../ECS.hpp"
#include "../Components.hpp"

/**
    Temporary Entity, which is only created for an animation (for example, a death animation),
    and will be destroyed after the animation is complete. 
*/
struct TempEntity : public Entity{

    TempEntity(
        Manager& man, std::string name,
        vec position,
        std::string sprite_name, std::string animation_name,
        float x_scale, float y_scale,
        SDL_RendererFlip sprite_flip
    );

    void render() override;
};