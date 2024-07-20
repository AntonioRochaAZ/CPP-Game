#include "Projectile.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../../Game.hpp"

using vec = Eigen::Vector2f;

/** Constructor
    @param rng: The range of the projectile before it is destroyed (in pixels).
    @param owner: The Entity that has shot the projectile. This is passed to the
        Damage component's immune vector (so that entity cannot hit itself).
*/
Projectile::Projectile(
    Manager& man, std::string name, vec position, float speed, vec velocity, 
    std::string sprite_name, float sprite_scale, float damage, int rng, Entity* owner
): Entity(man, name), range(rng), distance(0) {

    // Adding components:
    addComponent<Collider>(get_name(), DESTROY_ON_COLLISION); // Note: no shape specified
    addComponent<Transform>(position, speed, velocity);
    addComponent<Sprite>(sprite_name);
    addComponent<Damage>(damage);

    // Setting component information:
    getComponent<Collider>().enable_dynamic_shape();
    getComponent<Sprite>().set_scale(sprite_scale);
    if (owner != nullptr){ getComponent<Damage>().immune.emplace_back(owner); }
    
    // Dealing with manager:
    add_group(AttackGroup);
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