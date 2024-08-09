#include <filesystem>
#include "Game.hpp"

constexpr int window_width = 1200, window_height = 1000;
constexpr bool fullscreen = false;

int main(int argc, const char* argv[]) {

    // Correction for allowing running the executable by double clicking:    
    #ifndef WORK_DIR
        #define WORK_DIR "";
        // The name of the executable, which should be defined during compilation
        throw std::runtime_error("Work directory is not defined.");
    #endif
    std::filesystem::current_path(WORK_DIR);

    std::cout << std::filesystem::current_path() << std::endl;
    for (int i = 0; i < argc; i ++){
        std::cout << argv[i] << std::endl;
    }


    // Controlling frame rate
    constexpr int FPS = 30;
    constexpr int frame_delay = 1000/FPS;   // Max time between frames
    Uint64 frame_start;                 //
    int frame_time;                     //

    /* Trying parallelism.
    #pragma omp parallel for
    for(int i = 0; i < 10; i++){
        std::cout << "Thread nb. " << omp_get_thread_num() << "i:" << i << std::endl;
    }*/
    
    Game game;
    // TODO: Segmentation fault when clicking the executable comes from game->init instruction below:
    game.init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, fullscreen);
    while (game.running()){

        frame_start = SDL_GetTicks64(); // How much time (in ms) since we started SDL.
        game.handle_events();
        game.update();
        game.render();

        frame_time = SDL_GetTicks64() - frame_start; // How much time between frame_start and now.

        if (frame_delay > frame_time){
            // Then we need to delay the rendering
            SDL_Delay(frame_delay - frame_time);
        }else{
            #ifdef DEBUG_MODE
                std::cout << "Sloww..." << std::endl;
            #endif
        }
    }
    game.clean();
    #ifdef DEBUG_MODE
        std::cout << "Got to the end!" << std::endl;
    #endif
    return 0;
}