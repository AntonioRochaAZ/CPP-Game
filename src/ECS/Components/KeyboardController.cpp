#include "KeyboardController.hpp"
#include "Collider.hpp"

void KeyboardController::update(){
    // Check if a key is pressed:
    if (Game::event.type == SDL_KEYDOWN){
        switch (Game::event.key.keysym.sym){
        case SDLK_w:
            transform->set_vy(-1.0);
            sprite->set_animation("Walk");
            break;
        case SDLK_a:
            transform->set_vx(-1.0);
            sprite->set_animation("Walk");
            if (reverse_sprite){
                sprite->sprite_flip = SDL_FLIP_HORIZONTAL;
            }
            break;
        case SDLK_s:
            transform->set_vy(1.0);
            sprite->set_animation("Walk");
            break;
        case SDLK_d:
            transform->set_vx(1.0);
            sprite->set_animation("Walk");
            if (reverse_sprite){
                // Adding the if just to avoid problems.
                sprite->sprite_flip = SDL_FLIP_NONE;
            }
            break;
        case SDLK_k:
            if (!projectile && entity->has_group(PlayerGroup)){   
                // For some reason, multiple projectiles were being created,
                // resulting in segmentation fault. I've added this bool for the
                // time being.
                projectile = true;
                // In case a projectile is sent:
                Projectile* proj = new Projectile(
                    entity->manager, "Projectile", "projectile1", 
                    Sprite::Animation(1, 1000, 6, 3), 1000, 
                    transform->get_position()+vec(100, 0), 20, vec(1.0, 0.0)
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
    // Check if a key is released:
    if (Game::event.type == SDL_KEYUP){
        switch (Game::event.key.keysym.sym){
        case SDLK_w:
            transform->set_vy(0.0);
            break;
        case SDLK_a:
            transform->set_vx(0.0);
            break;
        case SDLK_s:
            transform->set_vy(0.0);
            break;
        case SDLK_d:
            transform->set_vx(0.0);
            break;
        case SDLK_k:
            projectile=false;
            break;
        default:
            break;
    }}
    if (transform->get_velocity().norm() == 0){
        sprite->set_animation("Idle");
    }
}
