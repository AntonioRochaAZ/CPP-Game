#include "Spawning.hpp"
#include "../Components.hpp"

void Spawning::init(){
    entity->getComponent<Health>().set_invincibility(true);
}

void Spawning::custom_update(){
    if (!is_active()){
        entity->getComponent<Health>().set_invincibility(false);
    }
}