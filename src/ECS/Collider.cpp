#include "Collider.hpp"

void Collider::init(){
    // Check if the entity already has the Transform component:
    if (!entity->has_component<Transform>()){
        // If not, add it:
        entity->addComponent<Transform>();
    }
    // Save the entity's transform as this class' transform member.
    transform = &entity->getComponent<Transform>();

    // Add to the Game's list of colliders:
    Game::collider_vector.push_back(this);
}
