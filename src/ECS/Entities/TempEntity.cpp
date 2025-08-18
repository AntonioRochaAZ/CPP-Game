#include "TempEntity.hpp"

TempEntity::TempEntity(
    Manager& man, std::string name, vec position, std::string sprite_name, 
    std::string animation_name, float x_scale, float y_scale, 
    SDL_RendererFlip sprite_flip
): Entity(man, name){

    // Adding Components:
    addComponent<Transform>(position[0], position[1]);
    addComponent<Sprite>(sprite_name);
    
    getComponent<Sprite>().set_animation(animation_name);
    getComponent<Sprite>().set_xscale(x_scale);
    getComponent<Sprite>().set_yscale(y_scale);
    getComponent<Sprite>().sprite_flip = sprite_flip;

    // Dealing with manager:
    add_group(TEMPORARY_GROUP);
}


// We do this check after the render, had we done this during the update, the entity would be 
// deleted before the last frame is rendered.
void TempEntity::render(){

    // Taking 
    int end = components.size();    // Avoid size changes during loop
    for (int i = 0; i < end; i++) { components[i]->render(); }

    // We must add 1 because current_frame starts at 0.
    if (getComponent<Sprite>().current_frame + 1 == getComponent<Sprite>().frames){
        // We've just rendered the last frame
        destroy();
    }

}