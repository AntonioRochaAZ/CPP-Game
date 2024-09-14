#pragma once
#include "../ECS/Components/KeyboardController.hpp"

/* Component for allowing controlling entities through the keyboard. */
struct KeyboardPlayer : public KeyboardController{

    bool reverse_sprite;    ///< Whether to reverse the sprite (left-right).
    bool projectile = false;    ///< For avoiding creating projectiles continuously.
    Transform* transform;
    Sprite* sprite;

    KeyboardPlayer(bool mReverse_sprite=true): reverse_sprite(mReverse_sprite){};
    
    /** Function for getting the Transform and Sprite components. This is separate from the init
    function in case the KeyboardController is defined before those other components and we have
    to get them later.*/
    void get_components(){
        if (entity->has_component<Transform>()){ transform = &entity->getComponent<Transform>(); }
        if (entity->has_component<Sprite>()){ sprite = &entity->getComponent<Sprite>(); }
    }

    void on_up() override;
    void on_down() override;
    void on_left() override;
    void on_right() override;
    // void on_attack_a() override;
    void on_attack_b() override;
    void on_up_end() override;
    void on_down_end() override;
    void on_left_end() override;
    void on_right_end() override;
    // void on_attack_a_end() override;
    // void on_attack_b_end() override;
    void on_update_end() override;
};
