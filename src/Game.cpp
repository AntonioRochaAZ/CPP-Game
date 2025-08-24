#include "Game.hpp"
#include "TileMap.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"
#include <sstream>
#include "ECS/Components/KeyboardController.hpp"
#include "utils.hpp"
#include "ECS/Entities/Player.hpp"
#include "ECS/Components/CustomControllers.hpp"

// Definition of window options:
constexpr float window_aspect_ratio = 16.0/9.0;
constexpr int  initial_window_height = 400;
constexpr int  initial_window_width = window_aspect_ratio * initial_window_height;
constexpr bool fullscreen = false;

// From KeyboardController.hpp:
std::map<int, KeyBind> global_key_bind_map;

//Map* tile_map;
Manager manager;
TileMap background("Background", 100, 100);

/// Definition of Game object's static variables:
SDL_Window* Game::m_window = nullptr;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<Collider*> Game::collider_vector;
bool Game::tracking_player = false;
Game::AssetManager Game::assets = Game::AssetManager();
vec Game::camera_position = vec(0.0, 0.0);

std::shared_ptr<Entity> message = manager.addEntity("Message");
std::shared_ptr<Entity> Game::cursor = manager.addEntity("CURSOR"); // DO NOT CHANGE THIS NAME OR DELETE THIS ENTITY
std::shared_ptr<Entity>& cursor = Game::cursor;

int Game::audio_volume =  (MIX_MAX_VOLUME * 50)/100; // Initializing volume at 50%
int Game::music_volume =  (MIX_MAX_VOLUME * 50)/100; // Initializing volume at 50%

// Constructor and destructor:
Game::Game(){};
Game::~Game(){};

void init_players(){
    // Setting up player 1:
    std::shared_ptr<Entity> player1 = manager.addEntity(std::make_shared<Player>(manager, "player1", "player1"));
    player1->getComponent<Transform>().set_position(150, 500);
    
    // Setting up player 2:
    std::shared_ptr<Entity> player2 = manager.addEntity(std::make_shared<Player>(manager, "player2", "player2"));
    std::map<int, KeyBind> player2_keybinds = {
        {SDLK_i, KeyBind::UP},
        {SDLK_k, KeyBind::DOWN},
        {SDLK_j, KeyBind::LEFT},
        {SDLK_l, KeyBind::RIGHT},
        {SDLK_o, KeyBind::ATTACK_A},
        {SDLK_p, KeyBind::ATTACK_B}
        // {SDLK_m, KeyBind::CAMERA_TOGGLE},
        // {SDLK_ESCAPE, KeyBind::QUIT}
    };
    player2->getComponent<KeyboardPlayer>().local_key_bind_map = player2_keybinds;
    player2->getComponent<Transform>().set_position(970, 500);
    player2->getComponent<Sprite>().sprite_flip = SDL_FLIP_HORIZONTAL;
}

// Initialize
int Game::init(const char* title){

    int window_flags = 0;   // Window creation flags
    is_running = false;     // Initialize is_running as false:

    #ifdef DEBUG_MODE
        std::cout << "Initializing SDL..." << std::endl;
    #endif 
    // Initialization check of SDL:
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL Initialization problem.";
        return 1;
    };

    #ifdef DEBUG_MODE
        std::cout << "Initializing SDL_TTF..." << std::endl;
    #endif 
    if(TTF_Init() != 0){
        std::cout << "SDL_TTF initialization error.";
        return 1;
    }



    #ifdef DEBUG_MODE
        std::cout << "Initializing Window..." << std::endl;
    #endif 
    // Window creation:
    if(fullscreen){
        window_flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE;
    }else{
        window_flags = SDL_WINDOW_RESIZABLE;
    };

    // Define m_window pointer: Title, x, y, width, height, window_flags
    m_window = SDL_CreateWindow(
        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initial_window_width, initial_window_height,
        window_flags
    );

    // Check for m_window:
    if(m_window == nullptr){
        std::cout << "Window creation problem.";
        return 2;
    }


    #ifdef DEBUG_MODE
        std::cout << "Initializing Renderer..." << std::endl;
    #endif 
    // Create renderer: m_window, index, flags                          
    renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(!renderer){
        std::cout << "Renderer creation problem.";
        return 3;
    };

    // TODO: create Loading screen here while assets are still being loaded!
    // We can like, make a new thread and load stuff there, while rerturning this
    // function so that the main loop can start (necessary for updating the loading screen)
    // Or maybe we can update the loading screen manually from here really... probably easier
    // honestly, but this may not work great for other loading screens outside of initialzation
    // must think.
    // To avoid players etc. form moving etc. before the end of the loading screen, we can add
    // and "active" member to the manager class, such that it will only update and render stuff
    // if it is active. Finally, we can activate it when loading is finished.   

    #ifdef DEBUG_MODE
        std::cout << "Initializing Rest..." << std::endl;
    #endif 
    // Set key maps up (must be done before any KeyboardController component is created.):
    global_key_bind_map.emplace(SDLK_w, KeyBind::UP);
    global_key_bind_map.emplace(SDLK_s, KeyBind::DOWN);
    global_key_bind_map.emplace(SDLK_a, KeyBind::LEFT);
    global_key_bind_map.emplace(SDLK_d, KeyBind::RIGHT);
    global_key_bind_map.emplace(SDLK_e, KeyBind::ATTACK_A);
    global_key_bind_map.emplace(SDLK_r, KeyBind::ATTACK_B);
    global_key_bind_map.emplace(SDLK_m, KeyBind::CAMERA_TOGGLE);
    global_key_bind_map.emplace(SDLK_1, KeyBind::RESET);
    global_key_bind_map.emplace(SDLK_ESCAPE, KeyBind::QUIT);

    // Change color of the renderer (black by default)
    // renderer, r, g, b, alpha (255 is opaque, 0 is transparent)
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);
    // Setting up some textures and the font:
    Game::assets.add_texture("player1", "./assets/player1.bmp");
    Game::assets.add_texture("player2", "./assets/player2.bmp");
    Game::assets.add_texture("projectile1", "./assets/projectile1-still.bmp");
    Game::assets.add_font("custom_font1px", "./assets/fonts/customfont-1px_spacing.ttf", 16);
    
    init_players();

    // Setting up background:
    Game::assets.add_texture("tiles", "./assets/tilemap.bmp");
    background.init("tiles", "./assets/tilemap_metadata.txt");
    background.load_map("./assets/map_3.txt");
    // Setting up background text:
    SDL_Color purple = { 255, 0, 255, 255 };
    std::shared_ptr<Entity>& label(manager.addEntity("TestLabel"));
    label->add_group(MAP_GROUP);
    label->addComponent<Transform>();
    label->addComponent<UILabel>(0, 0.0, 40, 20, "FIGHT!", "custom_font1px", purple, 600);
    // I prefered this manual spot rather than the centered one:
    label->getComponent<Transform>().position = vec(250, 200);

    SDL_Color ocean_blue = { 52, 143, 235, 255 };
    message->add_group(MAP_GROUP);
    message->addComponent<Transform>();
    message->addComponent<UILabel>(0, 0.0, 40, 20, "", "custom_font1px", ocean_blue, 96);
    int text_w, text_h;
    int text_ret = Game::assets.get_text_center_position(
        "custom_font1px",
        "Press 1 to reset!",
        &text_w, &text_h
    );
    if (text_ret == 0){
        message->getComponent<Transform>().position[0] = text_w;
        message->getComponent<Transform>().position[1] = 200;
    }else{
        message->getComponent<Transform>().position = vec(400, 200);
    }

    cursor->add_group(COLLIDER_GROUP);
    cursor->addComponent<Transform>();
    cursor->addComponent<Collider>("Cursor", CollisionHandle::PHASE, 10, 10);
    cursor->getComponent<Collider>().visualize_collider = true;

    // Load audio:
    // Initializing SDL_mixer:
    int status_code = Mix_Init(MIX_INIT_MP3);
    if (status_code == 0) { std::cout << "Error initializing SDL_Mixer" << std::endl; throw std::runtime_error(""); };
    // Initializing SDL_Mixer audio device:
    status_code = Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 2048 ); // Following SDL_mixer recommendations: https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudio
    if (status_code < 0) { std::cout << "Error opening Audio device" << std::endl; throw std::runtime_error(""); };
    Mix_Volume(-1, Game::audio_volume);     // Volume members initialized at the beginning of this file as 50%
    Mix_VolumeMusic(Game::music_volume);

    // Load music:
    Game::assets.add_music("music1", "./assets/audio/428858__supervanz__duskwalkin_loop.wav");
    Game::assets.add_audio("laser", "./assets/audio/515733__lilmati__retro-laser-shot-07.wav");
    Game::assets.add_audio("death", "./assets/audio/414209__jacksonacademyashmore__death.wav");

    Mix_PlayMusic(Game::assets.get_music("music1"), -1);    // -1 --> Infinite loop.
    // Ok, it is running now:
    is_running = true;
    #ifdef DEBUG_MODE
        std::cout << "Finished initialization!" << std::endl;
    #endif
    return 0;
};

// Handle events:
void Game::handle_events(){
    bool key_is_bound;
    
    while (SDL_PollEvent(&event) != 0){
        
        manager.handle_events(&event);

        switch (event.type){
        case SDL_KEYDOWN:
            // Check if the key is bound: DO NOT WRAP DEBUG_MODE AROUND NEXT LINE
            key_is_bound = check_map_id<int, KeyBind>(
                    global_key_bind_map, event.key.keysym.sym, "", false);    // Message unnecessary since we'll not halt execution
            if (key_is_bound){  // If so, go to switch statement:
            KeyBind pressed_key = global_key_bind_map[event.key.keysym.sym];
            switch (pressed_key){
                case KeyBind::CAMERA_TOGGLE:
                    if (Game::tracking_player){
                        // This condition must be met, otherwise
                        // will have a bug where neither the map not
                        // the player doesn't move
                        Game::tracking_player = false;
                    }else{
                        Game::tracking_player = true;
                        // Get current reference camera and entity positions (will be update next):
                        previous_camera_position = camera_position;

                        // Deal with camera_ref_entity if the reference entity has died:
                        if (camera_ref_entity.expired()){
                            update_camera_ref_entity();
                        }
                        // If it isn't anymore, update position.
                        if (!camera_ref_entity.expired()){ 
                            previous_ref_entity_position = camera_ref_entity.lock()->getComponent<Transform>().position;
                        }
                    }
                    break;
                case KeyBind::RESET:
                    // Reset players:
                    for (Entity* e_ptr : manager.grouped_entities[PLAYER_GROUP]){
                        // Case object from Entity to Player so we can call custom 
                        // destroy method, which allow destruction without the death
                        // animation:
                        Player* p_ptr = dynamic_cast<Player*>(e_ptr);
                        p_ptr->destroy(false); 
                    }
                    manager.refresh();
                    init_players();
                    // Reset camera position:
                    camera_position = vec(0,0);
                    message->getComponent<UILabel>().set_text("", "custom_font1px");
                    break;
                case KeyBind::QUIT:
                    is_running = false;
                    break;
                default:
                    break;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            cursor->getComponent<Transform>().position = vec(event.motion.x, event.motion.y);
            // Collider *cursor_collider = cursor->getComponent<Collider>();
            // // Check if it collides with an entity that has the MouseController component:
            // for (Collider *c : Game::collider_vector){
            //     if (Collision::collider_AABB(c, )){

            //     }

            // }

            break;
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event){
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                int w_width, w_height;
                SDL_GetWindowSize(m_window, &w_width, &w_height);
                #ifdef DEBUG_MODE
                    Mix_PlayChannel(-1, Game::assets.get_audio("death"), 0);
                    std::cout << "New window size: " << w_width << " " << w_height << std::endl;
                #endif
                break;
            default:
                #ifdef DEBUG_MODE
                    //Mix_PlayChannel(-1, Game::assets.get_audio("laser"), 0);
                #endif
                break;
            }


            break;
        default:
            break;
        }
    }
};

void Game::update(){
    update_counter++;

    background.update();
    manager.update();

    Collision::handle_collisions();     // Handling positions
    manager.refresh();
        //< In case entities have died after collisions and we don't want
        //< to render them etc.

    // Updating camera position:
    if (tracking_player){ 
        if (camera_ref_entity.expired()){ update_camera_ref_entity(); }
        // If it isn't nullptr:
        if (!camera_ref_entity.expired()){
            // Getting new position:
            vec new_ref_entity_position = camera_ref_entity.lock()->getComponent<Transform>().position;
            // Updating camera position:
            camera_position = previous_camera_position + \
                (new_ref_entity_position - previous_ref_entity_position);

            // Updating previous camera position:
            previous_camera_position = camera_position;
            // Updating ref_entity_position:k
            previous_ref_entity_position = new_ref_entity_position;
        }
    }

    if (manager.grouped_entities[PLAYER_GROUP].size() == 1){
        // Only one player left: add a message telling the players to reset.
        message->getComponent<UILabel>().set_text("Press 1 to reset!", "custom_font1px");
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
    background.render();
    manager.render();
    // Small bypass to ensure the player shows up in the screen:
    // player.getComponent<Sprite>().render();
    // Not sure why this is necessary, but should be the last:
    SDL_RenderPresent(renderer);
    // Delay so we can se the window (3000ms = 3s):
    // SDL_Delay(3000);
};


// Destroy objects
void Game::clean(){
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    SDL_Quit();
    is_running = false; //Just in case
    //delete tile_map;
};


void Game::update_camera_ref_entity(){
    // Try to get a new player as the reference entity:
    if (manager.grouped_entities[PLAYER_GROUP].size() != 0){
        // I think here this won't work: we would need to reach out for the
        // regular shared pointer in the manager's entity vector.
        Entity*& player_ptr =  manager.grouped_entities[PLAYER_GROUP][0];
        std::shared_ptr<Entity> found_ptr;
        for (std::shared_ptr<Entity> sh_ptr : manager.entity_vector){
            if (sh_ptr.get() == player_ptr){
                found_ptr = sh_ptr;
                break;
            }
        }
        camera_ref_entity = found_ptr;

    }else{
        Game::tracking_player = false;
    }
}