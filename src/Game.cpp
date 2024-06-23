#include "Game.hpp"
#include "TileMap.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"
#include <sstream>
#include "ECS/Components/KeyboardController.hpp"
#include "utils.hpp"

// From KeyboardController.hpp:
std::map<int, std::size_t> global_key_bind_map;

//Map* tile_map;
Manager manager;
TileMap background = TileMap("Background");

/// Definition of Game object's static variables:
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<Collider*> Game::collider_vector;
bool Game::tracking_player = false;
Game::AssetManager Game::assets = Game::AssetManager();
vec Game::camera_position = vec(0.0, 0.0);

auto& player(manager.addEntity("Player 1"));
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
    global_key_bind_map.emplace(SDLK_j, ATTACK_A_BUTTON);
    global_key_bind_map.emplace(SDLK_k, ATTACK_B_BUTTON);
    global_key_bind_map.emplace(SDLK_p, CAMERA_TOGGLE_BUTTON);
    global_key_bind_map.emplace(SDLK_ESCAPE, QUIT_BUTTON);

    // Change color of the renderer (black by default)
    // renderer, r, g, b, alpha (255 is opaque, 0 is transparent)
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);

    Game::assets.add_texture("player1", "./assets/player1.bmp");
    Game::assets.add_texture("player2", "./assets/player2.bmp");
    Game::assets.add_texture("projectile1", "./assets/projectile1-still.bmp");
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>(
        player.get_name(), 80, 170, MOVABLE_OBJECT);
    player.addComponent<Transform>(vec(0.0, 0.0), 10, vec(0.0, 0.0));
    player.addComponent<Sprite>("player1");
    player.getComponent<KeyboardController>().get_components();   // Force it to get the Transform and Sprite pointers.
    // Game::assets.add_font("andale", "./assets/fonts/andale_mono.ttf", 16);
    Game::assets.add_font("custom_font2px", "./assets/fonts/customfont-2px_spacing.ttf", 16);
    SDL_Color red = { 255, 0, 0, 255 };
    player.addComponent<UILabel>(20.0, -30.0, 40, 20, "P1", "custom_font2px", red, 24);
    // player.addComponent<Collider>(
    //     player.get_name(), player.getComponent<Sprite>().get_dst_width(),
    //     player.getComponent<Sprite>().get_dst_height(), MOVABLE_OBJECT);
    player.add_group(PlayerGroup);

    //player.getComponent<Sprite>().init();
    player.getComponent<Collider>().get_components();
    player.getComponent<Transform>().set_position(0, 0);
    // Animation idle_animation(2, 1500, 8, 17);
    // Animation walk_animation(5, 100, 9, 17);
    // player.getComponent<Sprite>().add_animation(idle_animation, "Idle");
    // player.getComponent<Sprite>().add_animation(walk_animation, "Walk");
    player.getComponent<Sprite>().set_animation("Idle");
    player.getComponent<Sprite>().set_scale(10);

    Game::assets.add_texture("tiles", "./assets/textures.bmp");
    background.init(10, 10, 2, 10, Game::assets.get_tuple("tiles"));
    background.set_dst_size(100, 100);
    background.setup(10, 10);
    background.load_map("./assets/map_1.txt");

    // Font:
    SDL_Color white = { 255, 255, 255, 255 };
    label.addComponent<Transform>();
    label.addComponent<UILabel>(-400.0, 0.0, 40, 20, "Hello there", "custom_font2px", white, 24);
    
    // Setting the reference entity (for camera tracking) as the player:
    camera_ref_entity = &player;

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
                        player.getComponent<Sprite>().set_texture("player1");
                    }else{
                        Game::tracking_player = true;
                        // Get current reference camera and entity positions (will be update next):
                        previous_camera_position = camera_position;
                        previous_ref_entity_position = camera_ref_entity->getComponent<Transform>().get_position();
                        player.getComponent<Sprite>().set_texture("player2");
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
    ss << "Player position: " << player.getComponent<Transform>().get_x() << \
        ", " << player.getComponent<Transform>().get_y();
    label.getComponent<UILabel>().set_text(ss.str(), "custom_font2px");

    background.update();
    manager.update();

    Collision::handle_collisions();     // Handling positions

    // Updating camera position:
    if (tracking_player){ 
        // Getting new position:
        vec new_ref_entity_position = camera_ref_entity->getComponent<Transform>().get_position();
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


