#include "Health.hpp"

/** 
    This function is defined separately of "update" because health updates will
    most likely comme from collisions rather than the update loop. In this case,
    it doesn't make sense to call it all the time.

    @param damage: Optional damage to inflict.
*/
void Health::update_hp(float damage){
    m_hp = m_hp - damage;
    if (m_hp <= 0){ entity->destroy(); }
}

void Health::update(){
    if (m_rate != 0){ 
        m_hp = std::min(m_hp + m_rate, m_max_hp);
        update_hp();
    }
}