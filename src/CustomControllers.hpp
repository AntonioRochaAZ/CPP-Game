#pragma once
#include "CustomControllers/KeyboardPlayer.hpp"
#include "CustomControllers/MouseTest.hpp"

constexpr int number_of_mice_controllers = 2;
extern const std::array<ComponentID, number_of_mice_controllers> controller_array;

ComponentID has_mouse_controller(Entity& entity);
    ///< Defined in CustomControllers.cpp

MouseController* get_custom_mouse_controller(Entity& entity, ComponentID controller_id);
    ///< Defined in CustomControllers.cpp