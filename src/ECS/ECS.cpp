#include "ECS.hpp"

void Entity::add_group(Group group){
    group_bitset[group] = true;
    manager.add_to_group(this, group);
}
