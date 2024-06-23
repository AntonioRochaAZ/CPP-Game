#include "Projectile.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../../Game.hpp"

using vec = Eigen::Vector2f;

Projectile::Projectile(Manager& man, std::string name, std::string sprite_name, int rng, vec position, float speed, vec velocity):
    Entity(man, name), range(rng), distance(0){
    
    this->addComponent<Collider>(name, 10, 10, DESTROY_ON_COLLISION);   // temporary width and height
    this->addComponent<Transform>(position, speed, velocity);
    this->addComponent<Sprite>(sprite_name);
    //this->getComponent<Sprite>().add_animation(charging, "Charging");

    this->getComponent<Collider>().get_components();
    this->getComponent<Collider>().set_width(this->getComponent<Sprite>().get_dst_width());
    this->getComponent<Collider>().set_height(this->getComponent<Sprite>().get_dst_height());
    
    // this->getComponent<Sprite>().add_animation(flying_animation, "Flying");
    // this->getComponent<Sprite>().set_animation("Flying");

    this->manager.addEntity(this);
    this->add_group(ProjectileGroup);   // Add this Entity to the Manager's ProjectileGroup.

};

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
    }else if (false){   // set to false because not implemented
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