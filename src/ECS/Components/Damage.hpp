#pragma once
#include "../ECS.hpp"

struct Damage : public Component{
    float damage;   ///< The damage it does (in health points).
    std::vector<Entity*> immune;  ///< Entities that should not be hurt by this attack. This can be used for having teams, or avoiding the original entity to get hurt from its own attack.

    Damage(float mDamage): damage(mDamage){};
};