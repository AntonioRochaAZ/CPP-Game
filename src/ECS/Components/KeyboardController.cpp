#include "KeyboardController.hpp"
#include "Collider.hpp"
#include "../../utils.hpp"

void KeyboardController::update(){
    // Check if a key is pressed:
    if (Game::event.type == SDL_KEYDOWN){
        // Check if the key is bound:
        bool key_is_bound = check_map_id<int, std::size_t>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        std::size_t pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case UP_BUTTON:
            transform->set_vy(-1.0);
            sprite->set_animation("Walk");
            break;
        case LEFT_BUTTON:
            transform->set_vx(-1.0);
            sprite->set_animation("Walk");
            if (reverse_sprite){
                sprite->sprite_flip = SDL_FLIP_HORIZONTAL;
            }
            break;
        case DOWN_BUTTON:
            transform->set_vy(1.0);
            sprite->set_animation("Walk");
            break;
        case RIGHT_BUTTON:
            transform->set_vx(1.0);
            sprite->set_animation("Walk");
            if (reverse_sprite){
                // Adding the if just to avoid problems.
                sprite->sprite_flip = SDL_FLIP_NONE;
            }
            break;
        case ATTACK_B_BUTTON:
            if (!projectile && entity->has_group(PlayerGroup)){   
                // For some reason, multiple projectiles were being created,
                // resulting in segmentation fault. I've added this bool for the
                // time being.
                projectile = true;
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
                    std::cout << "Error when trying to decide direction to shoot projectile." << std::endl;
                    std::cout << "Only coded for SDL_FLIP_NONE and SDL_FLIP_HORIZONTAL. Got this value instead: " \
                              << sprite->sprite_flip << std::endl;
                    throw std::runtime_error("");
                    break;
                }
                Projectile* proj = new Projectile(
                    entity->manager, "Projectile", "projectile1", 
                    Sprite::Animation(1, 1000, 6, 3), 1000, 
                    transform->get_position()+vec(x_sign * 100, 0), 20, vec(x_sign * 1.0, 0.0)
                );
                entity->manager.addEntity(proj);
                proj->getComponent<Sprite>().set_scale(10);   // TODO: Eventually get player scale.
                proj->getComponent<Collider>().get_shape();   // Updating collider dimensions.
                // Will this pointer ever be deleted??
                proj->add_group(ProjectileGroup);
            }
            break;
        default:
            break;
        }}
    }
    // Check if a key is released:
    if (Game::event.type == SDL_KEYUP){
        // Check if the key is bound:
        bool key_is_bound = check_map_id<int, std::size_t>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        std::size_t pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case UP_BUTTON:
            transform->set_vy(0.0);
            break;
        case LEFT_BUTTON:
            transform->set_vx(0.0);
            break;
        case DOWN_BUTTON:
            transform->set_vy(0.0);
            break;
        case RIGHT_BUTTON:
            transform->set_vx(0.0);
            break;
        case ATTACK_B_BUTTON:
            projectile=false;
            break;
        default:
            break;
        }}
    }
    if (transform->get_velocity().norm() == 0){
        sprite->set_animation("Idle");
    }
}
