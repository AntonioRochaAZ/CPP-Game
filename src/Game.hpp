// Definition of Game Class
// Methods are defined in Game.cpp


#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include "SDL2/SDL.h"
#include <vector>

class Collider;

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

    static void add_tile(int x, int y, int w, int h, int id);
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<Collider*> collider_vector;

private:
    int update_counter = 0;
    bool is_running;
    SDL_Window* window;

};

#endif /* Game_hpp */