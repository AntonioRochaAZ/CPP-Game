#include "Player.hpp"

std::unordered_map<int, SDL_Color> Player::player_colour = {
    {1, {255, 0  , 0  , 255}},  // red
    {2, {0  , 0  , 255, 255}},  // blue
    {3, {0  , 200, 0  , 255}},  // green
    {4, {255, 255, 0  , 255}}   // yellow
};

void Player::init(std::string sprite_name, float sprite_scale, float speed){
    
    // Adding components IN THE RIGHT UPDATE ORDER:
    addComponent<KeyboardController>();
    addComponent<Collider>(get_name(), 0, 0, MOVABLE_OBJECT); 
        // width and height will be updated later with the Sprite component.
    addComponent<Transform>(vec(0.0, 0.0), speed, vec(0.0, 0.0));
    addComponent<Sprite>(sprite_name);
    
    // Setting animations up:
    getComponent<Collider>().enable_dynamic_shape();    
        // Will allow us to get the shape from the Sprite component
    getComponent<Sprite>().set_scale(10);
    getComponent<Sprite>().set_animation("Idle");
        // This last line will automatically update the Collider's shape.

    // Now let's get Transform and Sprite information into earlier components:
    getComponent<KeyboardController>().get_components();   // Force it to get the Transform and Sprite pointers.
    
    // Dealing with manager:
    manager.addEntity(this);  // Add entity to Manager's entity vector.
    add_group(PlayerGroup);   // Add it to the PlayerGroup.

    // Get player number:
    player_number = manager.grouped_entities[PlayerGroup].size();
    if (player_number > player_colour.size()){
        std::cout << "Cannot have more than " << player_colour.size() << " players: the amount of colours in the player_colour map doesn't go that far." << std::endl;
        throw std::runtime_error("");
    }
    player_name = "P" + std::to_string(player_number);
    addComponent<UILabel>(20.0, -30.0, 40, 20, player_name, "custom_font2px", player_colour[player_number], 24);

}