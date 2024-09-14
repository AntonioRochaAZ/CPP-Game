#include "Collider.hpp"
#include <stdexcept>    // Exception throwing
#include "../../Collision.hpp"

Collider::~Collider(){
    // Removes itself from Game's collider_vector.
    Game::collider_vector.erase(
        std::remove(Game::collider_vector.begin(), Game::collider_vector.end(), this),
        Game::collider_vector.end()
    );
}

void Collider::init(){
    // Add to the Game's list of colliders:
    Game::collider_vector.push_back(this);
}

void Collider::get_shape(){ 
    width  = entity->getComponent<Sprite>().get_dst_width();
    height = entity->getComponent<Sprite>().get_dst_height();
}

void Collider::render(){
    if (visualize_collider){
        SDL_Rect rect;
        rect.x = entity->getComponent<Transform>().x;
        rect.y = entity->getComponent<Transform>().y;
        rect.w = width;
        rect.h = height;
        SDL_RenderDrawRect(Game::renderer, &rect);
    }
}