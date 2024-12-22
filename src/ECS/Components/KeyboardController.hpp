#pragma once
#include "../../Game.hpp"
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../Entities/Projectile.hpp"
#include "../States.hpp"

using vec = Eigen::Vector2f;

enum class KeyBind : std::size_t{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ATTACK_A,
    ATTACK_B,
    CAMERA_TOGGLE,
    RESET,
    QUIT,
};

extern std::map<int, KeyBind> global_key_bind_map;    ///< A map between the key_bind enum and the SDL_KeyCode.

/* Component for allowing controlling entities through the keyboard. */
struct KeyboardController : public Component{
    std::map<int, KeyBind> local_key_bind_map;    ///< A map between the key_bind enum and the SDL_KeyCode.

    KeyboardController(){};
    void init() override{ local_key_bind_map = global_key_bind_map; }

    virtual void handle_events(SDL_Event *event); ///< Defined in KeyboardController.cpp file.
    virtual void on_up(){};
    virtual void on_down(){};
    virtual void on_left(){};
    virtual void on_right(){};
    virtual void on_attack_a(){};
    virtual void on_attack_b(){};
    virtual void on_up_end(){};
    virtual void on_down_end(){};
    virtual void on_left_end(){};
    virtual void on_right_end(){};
    virtual void on_attack_a_end(){};
    virtual void on_attack_b_end(){};
    virtual void on_update_end(){};
};
