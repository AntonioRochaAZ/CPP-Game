#pragma once
#include "CustomControllers/KeyboardPlayer.hpp"
#include "CustomControllers/MouseTest.hpp"

constexpr int number_of_mice_controllers = 2;
extern const std::array<ComponentID, number_of_mice_controllers> mice_controller_array;

constexpr int number_of_keyboard_controllers = 2;
extern const std::array<ComponentID, number_of_keyboard_controllers> keyboard_controller_array;

ComponentID has_mouse_controller(Entity& entity);
    ///< Defined in CustomControllers.cpp

MouseController* get_custom_mouse_controller(Entity& entity, ComponentID controller_id);
    ///< Defined in CustomControllers.cpptroller_array;

ComponentID has_keyboard_controller(Entity& entity);
    ///< Defined in CustomControllers.cpp

KeyboardController* get_custom_keyboard_controller(Entity& entity, ComponentID controller_id);
    ///< Defined in CustomControllers.cpp