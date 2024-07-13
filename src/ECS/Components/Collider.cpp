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
    width  = entity->getComponent<Sprite>().dst_rect.w;
    height = entity->getComponent<Sprite>().dst_rect.h;
}