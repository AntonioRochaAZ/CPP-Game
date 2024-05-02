#include "Projectile.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../../Game.hpp"

using vec = Eigen::Vector2f;

Projectile::Projectile(Manager& man, std::string name, std::string sprite_name, Sprite::Animation fly, int rng, vec position, float speed, vec velocity):
    Entity(man, name), range(rng), distance(0), flying_animation(fly){
    // Animation charging(4, 20, 6, 3), flying(1, 10000, 6, 3);
    
    this->addComponent<Transform>(position, speed, velocity);
    this->addComponent<Sprite>(Game::assets.get_tuple(sprite_name), true);
    //this->getComponent<Sprite>().add_animation(charging, "Charging");
    this->getComponent<Sprite>().add_animation(flying_animation, "Flying");
    this->getComponent<Sprite>().set_animation("Flying");
};
/*
void Projectile::set_trajectory(Transform* ref_transform, int rng, float spd, vec vel){
    range = rng; reference_transform = ref_transform;
    speed = spd; velocity = vel;

    // We'll start by following the reference object:
    this->addComponent<Transform>(
        ref_transform->get_position(), 
        ref_transform->get_speed(), 
        ref_transform->get_velocity());
}
*/
void Projectile::update(){

    /*
    if (this->getComponent<Sprite>().current_animation == "Charging"
        && this->getComponent<Sprite>().current_frame+1 == charging.frames){
        // If we are in the charging animation and we've reached the final frame, switch
        // to the flying animation and stop following the reference object (if applicable).

        this->getComponent<Sprite>().set_animation("Flying");

        vec position = ref_transform.get_position();
        float ref_speed = ref_transform.get_speed();
        vec ref_velocity = ref_transform.get_velocity();
        velocity = speed * velocity + ref_speed * ref_velocity;
        speed = velocity.norm();

    } else if (this->getComponent<Sprite>().current_animation == "Charging"){
        // In this case, we are in the charging animaiton, but not yet in the last frame.
        // We'll follow the reference object:
        this->getComponent<Transform>().set_position(reference_transform->get_position())
        this->getComponent<Transform>().set_speed(reference_transform->get_speed())
        this->getComponent<Transform>().set_velocity(reference_transform->get_velocity())
    }
    */
    
    distance += this->getComponent<Transform>().get_speed();
    if (distance > range){
        // Projectile reaches its maximum range
        this->destroy();
    }else if (false){
        // Check if projectile went off-screen
        // (TODO: should also check if it hits a barrier)
        // Tutorial code:
        // We could perhaps use the collider code for this. Actually, it would
        // be the opposite (the Game camera bounding box and the projectile box
        // NOT colliding.)
        /*else if (transform->position.x > Game::camera.x + Game::camera.w ||
                   transform->position.x < Game::camera.x ||
                   transform->position.y > Game::camera.y + Game::camera.h ||
                   transform->position.y < Game::camera.y){
             
        }
        
        
        */
        // My code:
        //float proj_x, proj_y, window_x, window_y;
        //proj_x = this->getComponent<Transform>().get_x()
        
    }else{
        for (auto& c: components) c->update();
    }
};