#include "Game.hpp"
#include "TextureManager.hpp"
#include "TileMap.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"

//Map* tile_map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<Collider*> Game::collider_vector;

auto& player(manager.addEntity());
//auto& wall(manager.addEntity());

Game::Game(){};
Game::~Game(){};

// Initialize
int Game::init(const char* title, int x, int y, int width, int height, bool fullscreen){
    int window_flags = 0;   // Window creation flags

    // Initialize is_running as false:
    is_running = false;

    // Initialization check of SDL:
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL Initialization problem.";
        return 1;
    };

    // Window creation:
    if(fullscreen){
        window_flags = SDL_WINDOW_FULLSCREEN;
    }else{
        window_flags = SDL_WINDOW_SHOWN;
    };
    // Define window pointer: Title, x, y, width, height, window_flags
    window = SDL_CreateWindow(
        title, x, y, width, height, window_flags
    );
    // Check for window:
    if(!window){
        std::cout << "Window creation problem.";
        return 2;
    }

    // Create renderer: window, index, flags                          
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        std::cout << "Renderer creation problem.";
        return 3;
    };
    // Change color of the renderer (black by default)
    // renderer, r, g, b, alpha (255 is opaque, 0 is transparent)
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);

    // To create a texture we have to first have a surface, but
    // we can get rid of it later
    //background = new GameObject("assets/arena.bmp", 0, 0);
    //tile_map = new Map();
    //tile_map->init(width, height, 100, 100);
    //player = new GameObject("assets/test.bmp", 0, 0);
    //enemy = new GameObject("assets/test2.bmp", 100, 100);

    TileMap::load_map("assets/map_1.txt", 0, 0, 10, 10);

    player.addComponent<Transform>();
    player.addComponent<Sprite>("assets/test.bmp");
    player.getComponent<Sprite>().init();
    player.getComponent<Transform>().set_position(0, 0);
    player.getComponent<Transform>().set_height(100);
    player.getComponent<Transform>().set_width(100);
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>("Player");

    /*
    wall.addComponent<Transform>();
    wall.addComponent<Sprite>("assets/stone.bmp");
    wall.getComponent<Sprite>().init();
    wall.getComponent<Transform>().set_position(700, 800);
    wall.getComponent<Transform>().set_height(300);
    wall.getComponent<Transform>().set_width(20);
    wall.addComponent<Collider>("Wall");
    */

    // Ok, it is running now:
    is_running = true;
    return 0;
};

// Handle events:
void Game::handle_events(){
    
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT: 
            is_running = false;
            break;
        default:
            break;
    }
};

void Game::update(){
    update_counter++;
    manager.update();

    for (auto cl : collider_vector){
        Collision::collider_AABB(player.getComponent<Collider>(), *cl);
        //player.getComponent<Transform>().velocity = -1 * player.getComponent<Transform>().velocity;
    }
};

// Render the window:
void Game::render(){

    // Clear the display:
    SDL_RenderClear(renderer);

    // This is where we would add things to render. Whatever you
    // paint first, will be in the background.

    // SDL_RenderCopy takes a renderer pointer, a texture
    // pointer, information about the source and destination
    // rectangles (the former would crop the source image, and
    // the latter would define where to put it in the renderer).
    // By passing NULL to the latter two terms, we'll use all
    // images and also use all of the window.
    //tile_map->render();
    manager.render();
    // Small bypass to ensure the player shows up in the screen:
    player.getComponent<Sprite>().render();
    // Not sure why this is necessary, but should be the last:
    SDL_RenderPresent(renderer);
    // Delay so we can se the window (3000ms = 3s):
    // SDL_Delay(3000);
};

void Game::add_tile(int x, int y, int w, int h, int id){
    auto& tile(manager.addEntity());
    tile.addComponent<Tile>(x, y, w, h, id);
    tile.getComponent<Tile>().init();
};

// Destroy objects
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    is_running = false; //Just in case
    //delete tile_map;
};