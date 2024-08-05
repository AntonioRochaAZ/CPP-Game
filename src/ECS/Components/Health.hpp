#pragma once
#include "../ECS.hpp"



class Health : public Component {
private:
    bool m_invincible;  ///< Whether the entity is at an invincible state.

public:
    // Members
    float m_max_hp;     ///< Maximum HP possible.
    float m_hp;         ///< Health points.
    float m_rate;       ///< Rate of updating. 
    
    Health(float starter_hp, float max_hp, float rate = 0): 
        m_invincible(false), m_max_hp(max_hp), m_hp(starter_hp), m_rate(rate) {}; ///< Constructor
    
    void update() override; ///< Calls update_hp if rate != 0.
    void update_hp(float damage = 0);       ///< Updates hp according to rate, deletes entity if hp <= 0.
    void set_invincibility(bool value);     ///< Updating invincibility value.
};