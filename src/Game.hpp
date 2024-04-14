// Definition of Game Class
// Methods are defined in Game.cpp
#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include <vector>
#include <map>
#include <string>
#include <eigen3/Eigen/Dense>
#include "ECS/ECS.hpp"

using TexTup = std::tuple<SDL_Texture*, int, int>;

class Collider; // Forward declaration of the Collider class.

class AssetManager{
public:
    AssetManager(){};
    ~AssetManager();

    void add_texture(std::string id, std::string path);
    SDL_Texture* get_texture(std::string id);
    TexTup get_tuple(std::string id);

    // From the old TextureManager definition:
    static TexTup load_texture(std::string texture_file);

private:

    // I would like to use a shared pointer instead of a regular pointer here,
    // to try it out, but I am not sure how this would affect the destroy
    // texture function we must use to destroy textures. I'll use this then
    std::map< std::string, SDL_Texture* > texture_map;
    std::map< std::string, int > width_map;
    std::map< std::string, int > height_map;

};

class Game {
public:
    Game();
    ~Game();

    int init(const char* title, int x, int y, int width, int height, bool fullscreen);

    void handle_events();
    void update();          // Go through our game objects and update them all.
    void render();
    void clean();           // Free memory.

    bool running(){return is_running;};         // To tell if the game is still running.

    // static void add_tile(int x, int y, int w, int h, int id, std::string name);
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<Collider*> collider_vector;
    static bool tracking_player;
    static std::unique_ptr<AssetManager> assets;

private:
    bool is_running;
    int update_counter = 0;
    SDL_Window* window;

};