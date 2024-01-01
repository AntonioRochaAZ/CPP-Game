//#include <iostream>
//#include <SDL2/SDL.h>
#include "Game.hpp"
#include "TextureManager.hpp"

Game *game = nullptr;

const int window_width=1200, window_height=1000;
const bool fullscreen=false;

int main(int argc, const char * argv[]) {

    // Controlling frame rate
    const int FPS = 30;
    const int frame_delay = 1000/FPS;   // Max time between frames
    Uint32 frame_start;                 //
    int frame_time;                     //

    game = new Game();
    game->init("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, fullscreen);
    while (game->running()){

        frame_start = SDL_GetTicks(); // How much time (in ms) since we started SDL.
        game->handle_events();
        game->update();
        game->render();

        frame_time = SDL_GetTicks() - frame_start; // How much time between frame_start and now.

        if (frame_delay > frame_time){
            // Then we need to delay the rendering
            SDL_Delay(frame_delay - frame_time);
        }else{
            std::cout << "Sloww...";
        }
    }
    game->clean();
    delete game;
    return 0;
}