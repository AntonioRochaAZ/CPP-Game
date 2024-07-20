#pragma once
#include "../ECS.hpp"

struct Damage : public Component{
    float damage;   ///< The damage it does (in health points).
    std::vector<Entity*> immune; ///< Entities that should not be hurt by this attack.

    Damage(float mDamage): damage(mDamage){};
};