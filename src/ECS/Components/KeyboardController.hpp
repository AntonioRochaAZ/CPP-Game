#pragma once
#include "../../Game.hpp"
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../Entities/Projectile.hpp"

using vec = Eigen::Vector2f;

enum key_bind_enum : std::size_t{
    UP_BUTTON,
    DOWN_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    ATTACK_A_BUTTON,
    ATTACK_B_BUTTON,
    CAMERA_TOGGLE_BUTTON,
    QUIT_BUTTON
};

extern std::map<int, std::size_t> global_key_bind_map;    ///< A map between the key_bind enum and the SDL_KeyCode.

/* Component for allowing controlling entities through the keyboard. */
struct KeyboardController : public Component{
    std::map<int, std::size_t> local_key_bind_map;    ///< A map between the key_bind enum and the SDL_KeyCode.

    bool reverse_sprite;    ///< Whether to reverse the sprite (left-right).

    KeyboardController() : reverse_sprite(true){};
    KeyboardController(bool mReverse_sprite) : reverse_sprite(mReverse_sprite){};

    bool projectile = false;    ///< For avoiding creating projectiles continuously.

    Transform* transform;
    Sprite* sprite;

    void init() override{ local_key_bind_map = global_key_bind_map; }

    /** Function for getting the Transform and Sprite components. This is separate from the init
    function in case the KeyboardController is defined before those other components and we have
    to get them later.*/
    void get_components(){
        if (entity->has_component<Transform>()){ transform = &entity->getComponent<Transform>(); }
        if (entity->has_component<Sprite>()){ sprite = &entity->getComponent<Sprite>(); }
    }

    void update() override; ///< Defined in KeyboardController.cpp file.
};
