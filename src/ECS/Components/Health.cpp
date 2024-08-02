#include "Health.hpp"

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
    m_hp = std::min(m_hp - damage, m_max_hp);
    if (m_hp <= 0){ entity->destroy(); }
}

void Health::update(){
    if (m_rate != 0){ 
        update_hp(-m_rate);
    }
}