#include "Collider.hpp"
#include <stdexcept>    // Exception throwing
#include "../../Collision.hpp"

Collider::~Collider(){
    Game::collider_vector.erase(
        std::remove(Game::collider_vector.begin(), Game::collider_vector.end(), this),
        Game::collider_vector.end()
    );
}

void Collider::init(){
    // Check if the entity already has the Transform component:
    if (entity->has_component<Transform>()){
        // Get it:
        transform = &entity->getComponent<Transform>();
    }
    // Add to the Game's list of colliders:
    Game::collider_vector.push_back(this);
}

void Collider::get_transform(){ transform = &entity->getComponent<Transform>(); }

void Collider::get_shape(){ 
    width  = this->entity->getComponent<Sprite>().get_dst_width();
    height = this->entity->getComponent<Sprite>().get_dst_height();
}