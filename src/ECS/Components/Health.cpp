#include "Health.hpp"
#include "../States/Hurting.hpp"


/** 
    This function is defined separately of "update" because health updates will
    most likely comme from collisions rather than the update loop. In this case,
    it doesn't make sense to call it all the time.

    @param damage: Optional damage to inflict (could also be negative which would
        amount to healing).
*/
void Health::update_hp(float damage){
    // The following lines of code won't work because keyboardController will
    // reset the animation. Applying a filter over the texture would probably
    // be the best way of dealing with this.  
    // if (damage > 0) {   // damage > 0 --> Actual damage, not healing.
    //     Sprite& sprite = entity->getComponent<Sprite>();
    //     sprite.set_animation(sprite.current_animation + "_hurt", false);
    // }
    if (m_invincible && damage > 0){
        return; // Invincibility frame, can still heal though.
    }
    if (damage > 0){
        entity->addComponent<Hurting>(450); 
            //< 250s of invincibility (perhaps make this entity dependant eventually...)
            //< Could eventually add all this type of information to a json file. This should
            //< be different than the sprite .txt since that is dependant exclusively on the
            //< the animations, while we may want to have different characters with the same
            //< sprite but different Hurting options etc.
    }
    m_hp = std::min(m_hp - damage, m_max_hp);
    if (m_hp <= 0){ entity->destroy(); }
}

void Health::update(){
    if (m_rate != 0){ 
        update_hp(-m_rate);
    }
}

void Health::set_invincibility(bool value){
    #ifdef DEBUG_MODE
        if (value == m_invincible){
            std::cout << "Trying to set invincibility to " << value << " but it is already this value." << std::endl;
            throw std::runtime_error("Health::set_invincibility");
        }
    #endif
    m_invincible = value;
}