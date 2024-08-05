#include "Player.hpp"
#include "TempEntity.hpp"
#include <algorithm>

std::unordered_map<int, SDL_Color> Player::player_colour = {
    {1, {255, 0  , 0  , 255}},  // red
    {2, {0  , 0  , 255, 255}},  // blue
    {3, {0  , 200, 0  , 255}},  // green
    {4, {255, 255, 0  , 255}}   // yellow
};

std::array<bool, max_players> Player::allocated_players = []{
    std::array<bool, max_players> arr;
    std::fill(arr.begin(), arr.end(), false);
    return arr;
}();

Player::Player(
    Manager& man, std::string name, std::string sprite_name, float sprite_scale, float speed
): Entity(man, name){
    
    // Adding components IN THE RIGHT UPDATE ORDER:
    addComponent<KeyboardController>();
    addComponent<Collider>(get_name(), CollisionHandle::MOVABLE);  // width and height will be updated later with the Sprite component.
    addComponent<Transform>();
    addComponent<Sprite>(sprite_name);
    addComponent<Health>(3, 3);         // starting_hp = 3, max_hp = 3
    
    // Setting things up:
    getComponent<Transform>().speed = speed;
    getComponent<Collider>().enable_dynamic_shape();  // Will allow us to get the shape from the Sprite component
    getComponent<Sprite>().set_scale(sprite_scale);
    getComponent<Sprite>().set_animation("Idle");  // This last line will automatically update the Collider's shape.

    // Now let's get Transform and Sprite information into earlier components:
    getComponent<KeyboardController>().get_components();   // Force it to get the Transform and Sprite pointers.
    
    // Dealing with manager:
    // manager.addEntity(this);  // Add entity to Manager's entity vector.
    add_group(PLAYER_GROUP);   // Add it to the PLAYER_GROUP.

    // Get player number:
    player_number = 0;
    for (int i = 0; i < max_players; i++){
        if (!Player::allocated_players[i]){
            player_number = i + 1;  // Because player_number starts at 1 and i at 0.
            Player::allocated_players[i] = true;
            break;
        }
    }
    if (player_number == 0){
        std::cout << "Cannot have more than " << player_colour.size() << " players: the amount of colours in the player_colour map doesn't go that far." << std::endl;
        throw std::runtime_error("");
    }
    player_name = "P" + std::to_string(player_number);
    addComponent<UILabel>(20.0, -80.0, 40, 20, player_name, "custom_font1px", player_colour[player_number], 96);
    addComponent<Spawning>(2000);
}

void Player::destroy(){ destroy(true); }
void Player::destroy(bool create_temp_entity){
    active = false;
    Player::allocated_players[player_number - 1] = false;
    if (create_temp_entity){
        manager.addEntity(new TempEntity(
            manager, "temp_player", getComponent<Transform>().position,
            getComponent<Sprite>().m_texture_id, "Death",
            getComponent<Sprite>().get_xscale(), getComponent<Sprite>().get_yscale(),  
            getComponent<Sprite>().sprite_flip
        ));
    }
}