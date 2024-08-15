#include "KeyboardController.hpp"
#include "../../utils.hpp"

void KeyboardController::update(){
    // Check if a key is pressed:
    if (Game::event.type == SDL_KEYDOWN){
        // Check if the key is bound: DO NOT WRAP DEBUG_MODE AROUND THIS
        bool key_is_bound = check_map_id<int, KeyBind>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        KeyBind pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case KeyBind::UP:
            on_up();
            break;
        case KeyBind::DOWN:
            on_down();
            break;
        case KeyBind::LEFT:
            on_left();
            break;
        case KeyBind::RIGHT:
            on_right();
            break;
        case KeyBind::ATTACK_A:
            on_attack_a();
            break;
        case KeyBind::ATTACK_B:
            on_attack_b();
            break;
        default:
            break;
        }}
    }
    // Check if a key is released:
    if (Game::event.type == SDL_KEYUP){
        // Check if the key is bound: DO NOT WRAP DEBUG_MODE AROUND THIS
        bool key_is_bound = check_map_id<int, KeyBind>(
                local_key_bind_map, Game::event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
        if (key_is_bound){  // If so, go to switch statement:
        KeyBind pressed_key = local_key_bind_map[Game::event.key.keysym.sym];
        switch (pressed_key){
        case KeyBind::UP:
            on_up_end();
            break;
        case KeyBind::DOWN:
            on_down_end();
            break;
        case KeyBind::LEFT:
            on_left_end();
            break;
        case KeyBind::RIGHT:
            on_right_end();
            break;
        case KeyBind::ATTACK_A:
            on_attack_a_end();
            break;
        case KeyBind::ATTACK_B:
            on_attack_b_end();
            break;
        default:
            break;
        }}
    }
    on_update_end();
}
