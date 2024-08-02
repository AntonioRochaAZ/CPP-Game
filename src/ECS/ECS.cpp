#include "ECS.hpp"

Entity::Entity(Manager& user_manager, std::string mName): manager(user_manager), name(mName){
    // manager.addEntity(this);
}

void Entity::add_group(Group group){ 
    group_bitset[group] = true; 
    manager.add_to_group(this, group);
}

void Manager::refresh(){
    
    // Here, i is initialized with direct initialization.
    for (std::size_t i = 0; i < max_groups; i++){
        // We initialize variable v as entry i of the
        // group_entities member. Because we use auto,
        // the type will be understood directly from the
        // initialized value. Note it is a reference.
        std::vector<Entity*>& v = grouped_entities[i];
        // We do as below: we remove entries of the
        // vector (so, Entity pointers) that aren't
        // active OR that are not part of the group.
        v.erase(
            std::remove_if(std::begin(v), std::end(v),
            [i](Entity* mEntity){
                return !mEntity->is_active() || !mEntity->has_group(i);
            }),
            std::end(v)
        );
    }

    // Delete entities that are no longer active:
    auto end = std::end(entity_vector);
    auto beginning = std::remove_if(
            std::begin(entity_vector), std::end(entity_vector),
            [](const std::shared_ptr<Entity> &mEntity){
                return !mEntity->is_active();
            }
        );
    entity_vector.erase(beginning,  end);
}