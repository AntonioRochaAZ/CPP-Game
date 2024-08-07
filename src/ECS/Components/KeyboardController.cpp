#include "KeyboardController.hpp"
#include "Collider.hpp"
#include "../../utils.hpp"

void KeyboardController::update(){
    // Check if a key is pressed:
    if (Game::event.type == SDL_KEYDOWN){
        // Check if the key is bound: DO NOT WRAP DEBUG_MODE AROUND THIS
        bool key_is_bound = check_map_id<int, KeyBind>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        KeyBind pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case KeyBind::UP:
            transform->vy = -1.0;
            sprite->set_animation("Walk");
            break;
        case KeyBind::LEFT:
            transform->vx = -1.0;
            sprite->set_animation("Walk");
            if (reverse_sprite){
                sprite->sprite_flip = SDL_FLIP_HORIZONTAL;
            }
            break;
        case KeyBind::DOWN:
            transform->vy = 1.0;
            sprite->set_animation("Walk");
            break;
        case KeyBind::RIGHT:
            transform->vx = 1.0;
            sprite->set_animation("Walk");
            if (reverse_sprite){
                // Adding the if just to avoid problems.
                sprite->sprite_flip = SDL_FLIP_NONE;
            }
            break;
        case KeyBind::ATTACK_B:
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
            break;
        default:
            break;
        }}
    }
    // Check if a key is released:
    if (Game::event.type == SDL_KEYUP){
        // Check if the key is bound: DO NOT WRAP DEBUG_MODE AROUND THIS
        bool key_is_bound = check_map_id<int, KeyBind>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        KeyBind pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case KeyBind::UP:
            transform->vy = 0.0;
            break;
        case KeyBind::LEFT:
            transform->vx = 0.0;
            break;
        case KeyBind::DOWN:
            transform->vy = 0.0;
            break;
        case KeyBind::RIGHT:
            transform->vx = 0.0;
            break;
        case KeyBind::ATTACK_B:
            break;
        default:
            break;
        }}
    }
    if (transform->velocity.norm() == 0){
        sprite->set_animation("Idle");
    }
}
