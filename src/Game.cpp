#include "Game.hpp"
#include "TileMap.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"
#include <sstream>
#include "ECS/Components/KeyboardController.hpp"
#include "utils.hpp"
#include "ECS/Entities/Player.hpp"

// From KeyboardController.hpp:
std::map<int, std::size_t> global_key_bind_map;

//Map* tile_map;
Manager manager;
TileMap background("Background", 100, 100);

/// Definition of Game object's static variables:
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<Collider*> Game::collider_vector;
bool Game::tracking_player = false;
Game::AssetManager Game::assets = Game::AssetManager();
vec Game::camera_position = vec(0.0, 0.0);

Player* player = new Player(manager, "player1");
    //< This will then be turned into a unique_pointer when initialized
    //< (initialization will add this Entity to manager, which will then
    //< take care of it). If we don't use a pointer here, the object will
    //< be freed twice (once by the manager through the deletion of the
    //< unique_pointer, then by game when it goes out of scope (if I 
    //< understood the runtime error correctly)).
Player* player2 = new Player(manager, "player2");


auto& label(manager.addEntity("TestLabel"));
//auto& wall(manager.addEntity());


Game::Game(){};

Game::~Game(){};

// Initialize
int Game::init(const char* title, int x, int y, int width, int height, bool fullscreen){
    int window_flags = 0;   // Window creation flags
    is_running = false;     // Initialize is_running as false:

    // Initialization check of SDL:
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL Initialization problem.";
        return 1;
    };

    if(TTF_Init() != 0){
        std::cout << "SDL_TTF initialization error.";
        return 1;
    }

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

    // Set key maps up (must be done before any KeyboardController component is created.):
    global_key_bind_map.emplace(SDLK_w, UP_BUTTON);
    global_key_bind_map.emplace(SDLK_s, DOWN_BUTTON);
    global_key_bind_map.emplace(SDLK_a, LEFT_BUTTON);
    global_key_bind_map.emplace(SDLK_d, RIGHT_BUTTON);
    global_key_bind_map.emplace(SDLK_e, ATTACK_A_BUTTON);
    global_key_bind_map.emplace(SDLK_r, ATTACK_B_BUTTON);
    global_key_bind_map.emplace(SDLK_m, CAMERA_TOGGLE_BUTTON);
    global_key_bind_map.emplace(SDLK_ESCAPE, QUIT_BUTTON);

    // Change color of the renderer (black by default)
    // renderer, r, g, b, alpha (255 is opaque, 0 is transparent)
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);

    Game::assets.add_texture("player1", "./assets/player1.bmp");
    Game::assets.add_texture("player2", "./assets/player2.bmp");
    Game::assets.add_texture("projectile1", "./assets/projectile1-still.bmp");
    Game::assets.add_font("custom_font2px", "./assets/fonts/customfont-2px_spacing.ttf", 16);
    
    player->init("player1");
    player2->init("player2");
    std::map<int, std::size_t> player2_keybinds = {
        {SDLK_i, UP_BUTTON},
        {SDLK_k, DOWN_BUTTON},
        {SDLK_j, LEFT_BUTTON},
        {SDLK_l, RIGHT_BUTTON},
        {SDLK_o, ATTACK_A_BUTTON},
        {SDLK_p, ATTACK_B_BUTTON}
        // {SDLK_m, CAMERA_TOGGLE_BUTTON},
        // {SDLK_ESCAPE, QUIT_BUTTON}
    };
    player2->getComponent<KeyboardController>().local_key_bind_map = player2_keybinds;
    
    player->getComponent<Transform>().set_position(150, 500);
    player2->getComponent<Transform>().set_position(970, 500);
    player2->getComponent<Sprite>().sprite_flip = SDL_FLIP_HORIZONTAL;

    Game::assets.add_texture("tiles", "./assets/tilemap.bmp");
    background.init("tiles", "./assets/tilemap_metadata.txt");
    background.load_map("./assets/map_3.txt");

    // Font:
    SDL_Color white = { 255, 255, 255, 255 };
    label.addComponent<Transform>();
    label.addComponent<UILabel>(-400.0, 0.0, 40, 20, "Hello there", "custom_font2px", white, 24);
    
    // Setting the reference entity (for camera tracking) as the player:
    camera_ref_entity = player;

    // Ok, it is running now:
    is_running = true;
    return 0;
};

// Handle events:
void Game::handle_events(){
    
    SDL_PollEvent(&event);
    bool key_is_bound;
    switch (event.type){
        case SDL_KEYDOWN:
            // Check if the key is bound:
            key_is_bound = check_map_id<int, std::size_t>(
                    global_key_bind_map, event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
            if (key_is_bound){  // If so, go to switch statement:
            std::size_t pressed_key = global_key_bind_map[event.key.keysym.sym];
            switch (pressed_key){
                case CAMERA_TOGGLE_BUTTON:
                    if (Game::tracking_player){
                        // This condition must be met, otherwise
                        // will have a bug where neither the map not
                        // the player doesn't move
                        Game::tracking_player = false;
                        // player->getComponent<Sprite>().set_texture("player1");
                        // player->getComponent<Sprite>().set_scale(10);
                        // background.load_map("./assets/map_1.txt");
                        // background.set_position(100, 100);
                    }else{
                        Game::tracking_player = true;
                        // Get current reference camera and entity positions (will be update next):
                        previous_camera_position = camera_position;
                        previous_ref_entity_position = camera_ref_entity->getComponent<Transform>().position;
                        // player->getComponent<Sprite>().set_texture("player2");
                        // player->getComponent<Sprite>().set_scale(20);
                        // background.load_map("./assets/map_2.txt");
                        // background.set_position(0, 0);
                    }
                    break;
                    
                case QUIT_BUTTON:
                    is_running = false;
                    break;
                default:
                    break;
            }}
            break;
        case SDL_QUIT:
            is_running = false;
            break;
        default:
            break;
    }
};

void Game::update(){
    update_counter++;

    // Label updating (debugging):
    std::stringstream ss;
    ss << "Player position: " << player->getComponent<Transform>().x << \
        ", " << player->getComponent<Transform>().y;
    label.getComponent<UILabel>().set_text(ss.str(), "custom_font2px");

    background.update();
    manager.update();

    Collision::handle_collisions();     // Handling positions

    // Updating camera position:
    if (tracking_player){ 
        // Getting new position:
        vec new_ref_entity_position = camera_ref_entity->getComponent<Transform>().position;
        // Updating camera position:
        camera_position = previous_camera_position + \
            (new_ref_entity_position - previous_ref_entity_position);

        // Updating previous camera position:
        previous_camera_position = camera_position;
        // Updating ref_entity_position:
        previous_ref_entity_position = new_ref_entity_position;
    }
};

// Declaring variable tiles which will be of type
// std::vector<Entity*>
auto& tile_vector(manager.get_group(MapGroup));
auto& enemy_vector(manager.get_group(EnemyGroup));
auto& projectile_vector(manager.get_group(ProjectileGroup));
auto& player_vector(manager.get_group(PlayerGroup));

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
    background.render();
    manager.render();
    label.render();
    // Small bypass to ensure the player shows up in the screen:
    // player.getComponent<Sprite>().render();
    // Not sure why this is necessary, but should be the last:
    SDL_RenderPresent(renderer);
    // Delay so we can se the window (3000ms = 3s):
    // SDL_Delay(3000);
};

// void Game::add_tile(int x, int y, int w, int h, int id, std::string name){
//     Entity& tile(manager.addEntity(name));
//     tile.addComponent<Tile>(x, y, w, h, id);
//     tile.getComponent<Tile>().init();
//     tile.add_group(MapGroup);
// };

// Destroy objects
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    is_running = false; //Just in case
    //delete tile_map;
};


