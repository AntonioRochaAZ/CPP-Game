#pragma once
#include "../ECS.hpp"
#include <eigen3/Eigen/Dense>
#include "../Components.hpp"

using vec = Eigen::Vector2f;

constexpr int max_players = 4;

/** Player Entity.
*/
struct Player : public Entity{
    static std::array<bool, max_players> allocated_players;
    int player_number;
    std::string player_name;
    static std::unordered_map<int, SDL_Color> player_colour;
        ///< Colours associated to each player. Initialized in Player.cpp;

    /** Initializes the player instance with a given sprite.
        @param sprite_name: The projectile's texture id (stored in Game::assets).
        @param sprite_scale: The scale factor to pass to the \ref Sprite component.
        @param speed: The speed to pass to the \ref Transform component.
    */         
    Player(
        Manager& man, std::string name,
        std::string sprite_name, float sprite_scale = 10,
        float speed = 10
    );

    void destroy() override;
    // ~Player();
};