#include "Hurting.hpp"
#include "../Components.hpp"

void Hurting::init(){
    entity->getComponent<Health>().set_invincibility(true);
}

void Hurting::custom_update(){
    if (!is_active()){
        entity->getComponent<Health>().set_invincibility(false);
    }
}