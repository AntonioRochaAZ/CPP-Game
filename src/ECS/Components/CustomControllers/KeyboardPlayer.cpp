#include "KeyboardPlayer.hpp"

void KeyboardPlayer::on_up(){
    transform->vy = -1.0;
    sprite->set_animation("Walk"); 
}
void KeyboardPlayer::on_up_end(){ transform->vy = 0.0; }

void KeyboardPlayer::on_down(){
    transform->vy = 1.0;
    sprite->set_animation("Walk");
}
void KeyboardPlayer::on_down_end(){ transform->vy = 0.0; }

void KeyboardPlayer::on_left(){
    transform->vx = -1.0;
    sprite->set_animation("Walk");
    if (reverse_sprite){
        sprite->sprite_flip = SDL_FLIP_HORIZONTAL;
    }
}
void KeyboardPlayer::on_left_end(){ transform->vx = 0.0; }

void KeyboardPlayer::on_right(){
    transform->vx = 1.0;
    sprite->set_animation("Walk");
    if (reverse_sprite){
        // Adding the if just to avoid problems.
        sprite->sprite_flip = SDL_FLIP_NONE;
    }
}
void KeyboardPlayer::on_right_end(){ transform->vx = 0.0; }

void KeyboardPlayer::on_update_end(){
    if (transform->velocity.norm() == 0){
        sprite->set_animation("Idle");
    }
}

void KeyboardPlayer::on_attack_b(){
    if (entity->has_group(PLAYER_GROUP) && !entity->has_component<Shooting>()){
        // In case a projectile is sent:
        int x_sign;
        switch (sprite->sprite_flip){
        case SDL_FLIP_NONE:
            x_sign = 1;
            break;
        case SDL_FLIP_HORIZONTAL:
            x_sign = -1;
            break;
        default:
            #ifdef DEBUG_MODE
                std::cout << "Error when trying to decide direction to shoot projectile." << std::endl;
                std::cout << "Only coded for SDL_FLIP_NONE and SDL_FLIP_HORIZONTAL. Got this value instead: " \
                        << sprite->sprite_flip << std::endl;
                throw std::runtime_error("");
            #endif
            x_sign = 1;
            break;
        }
        entity->addComponent<Shooting>(250);   // 250 ms of cooldown
        entity->manager.addEntity(new Projectile(
            entity->manager, "Projectile",
            transform->position + vec(x_sign * 100, 0), 20, vec(x_sign * 1.0, 0.0),
            "projectile1", 10,
            1, 
            1000, entity
        ));
    }
}