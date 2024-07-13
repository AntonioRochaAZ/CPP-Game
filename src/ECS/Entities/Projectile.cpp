#include "Projectile.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../../Game.hpp"

using vec = Eigen::Vector2f;

Projectile::Projectile(Manager& man, std::string name, std::string sprite_name, int rng, vec position, float speed, vec velocity):
    Entity(man, name), range(rng), distance(0){
    
    addComponent<Collider>(name, 10, 10, DESTROY_ON_COLLISION);   // temporary width and height
    addComponent<Transform>(position, speed, velocity);
    addComponent<Sprite>(sprite_name);
    //getComponent<Sprite>().add_animation(charging, "Charging");

    getComponent<Collider>().enable_dynamic_shape();    // Automatically gets the shape from the Sprite component.
    
    // getComponent<Sprite>().add_animation(flying_animation, "Flying");
    // getComponent<Sprite>().set_animation("Flying");

    manager.addEntity(this);
    add_group(ProjectileGroup);   // Add this Entity to the Manager's ProjectileGroup.

};

void Projectile::update(){

    /*
    if (getComponent<Sprite>().current_animation == "Charging"
        && getComponent<Sprite>().current_frame+1 == charging.frames){
        // If we are in the charging animation and we've reached the final frame, switch
        // to the flying animation and stop following the reference object (if applicable).

        getComponent<Sprite>().set_animation("Flying");

        vec position = ref_transform.position;
        float ref_speed = ref_transform.speed;
        vec ref_velocity = ref_transform.velocity;
        velocity = speed * velocity + ref_speed * ref_velocity;
        speed = velocity.norm();

    } else if (getComponent<Sprite>().current_animation == "Charging"){
        // In this case, we are in the charging animaiton, but not yet in the last frame.
        // We'll follow the reference object:
        getComponent<Transform>().position = reference_transform->position;
        getComponent<Transform>().speed = reference_transform->speed;
        getComponent<Transform>().velocity = reference_transform->velocity;
    }
    */
    
    distance += getComponent<Transform>().speed;
    if (distance > range){
        // Projectile reaches its maximum range
        destroy();
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
        //proj_x = getComponent<Transform>().get_x()
        
    }else{
        for (auto& c: components) c->update();
    }
};