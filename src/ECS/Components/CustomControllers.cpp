#include "CustomControllers.hpp"

// constexpr int number_of_mice_controllers = 1;
const std::array<ComponentID, number_of_mice_controllers> controller_array = {
    getComponentTypeID<MouseController>(),
    getComponentTypeID<MouseTest>()
};

/// Check if an entity has a custom mouse controller and, 
/// if so, return its ID. Note that, if it has more than
/// one (which wouldn't be interesting), then the first
/// one will be returned.
ComponentID has_mouse_controller(Entity& entity){
    for (ComponentID id : controller_array){
        if (entity.component_bitset[id]){
            return id;
        }
    }
    return max_components + 1; // That way we are sure to not clash with an existing component ID
}

/// Get the mouse controller so we can handle events.
/// The way this is being used in Collision.cpp does not seem to be the most
/// efficient...
MouseController* get_custom_mouse_controller(Entity& entity, ComponentID controller_id){
    Component* controller = entity.component_array[controller_id];
    return static_cast<MouseController*>(controller);
}