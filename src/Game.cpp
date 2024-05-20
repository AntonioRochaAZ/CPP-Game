#include "Game.hpp"
#include "TileMap.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"
#include <sstream>


//Map* tile_map;
Manager manager;
TileMap background = TileMap("Background");

/// Definition of Game object's static variables:
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<Collider*> Game::collider_vector;
bool Game::tracking_player = true;
Game::AssetManager Game::assets = Game::AssetManager();

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
    // Change color of the renderer (black by default)
    // renderer, r, g, b, alpha (255 is opaque, 0 is transparent)
    SDL_SetRenderDrawColor(renderer, 0, 255, 120, 255);

    Game::assets.add_texture("player", "assets/player.bmp");
    Game::assets.add_texture("projectile1", "assets/projectile1-still.bmp");
    player.addComponent<KeyboardController>();
    player.addComponent<Collider>(
        player.get_name(), 80, 170, MOVABLE_OBJECT);
    player.addComponent<Transform>(vec(0.0, 0.0), 10, vec(0.0, 0.0));
    player.addComponent<Sprite>(Game::assets.get_tuple("player"), true);
    player.getComponent<KeyboardController>().init();   // Force it to get the Transform and Sprite pointers.
    Game::assets.add_font("andale", "assets/fonts/andale_mono.ttf", 16);
    SDL_Color red = { 255, 0, 0, 255 };
    player.addComponent<UILabel>(20.0, -30.0, 4000, 2000, "P1", "andale", red);
    // player.addComponent<Collider>(
    //     player.get_name(), player.getComponent<Sprite>().get_dst_width(),
    //     player.getComponent<Sprite>().get_dst_height(), MOVABLE_OBJECT);
    player.add_group(PlayerGroup);

    //player.getComponent<Sprite>().init();
    player.getComponent<Collider>().get_transform();
    player.getComponent<Transform>().set_position(0, 0);
    Sprite::Animation idle_animation(2, 1500, 8, 17);
    Sprite::Animation walk_animation(5, 100, 9, 17);
    player.getComponent<Sprite>().add_animation(idle_animation, "Idle");
    player.getComponent<Sprite>().add_animation(walk_animation, "Walk");
    player.getComponent<Sprite>().set_animation("Idle");
    player.getComponent<Sprite>().set_scale(10);

    Game::assets.add_texture("tiles", "assets/textures.bmp");
    background.init(10, 10, 2, 10, Game::assets.get_tuple("tiles"));
    background.set_dst_size(100, 100);
    background.setup(10, 10);
    background.load_map("assets/map_1.txt");

    background.tile_player = &player;

    // Font:
    SDL_Color white = { 255, 255, 255, 255 };
    label.addComponent<Transform>();
    label.addComponent<UILabel>(0.0, 0.0, 4000, 2000, "Hello there", "andale", white);

    // Ok, it is running now:
    is_running = true;
    return 0;
};

// Handle events:
void Game::handle_events(){
    
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym){
                case SDLK_p:
                    if (Game::tracking_player){
                        // This condition must be met, otherwise
                        // will have a bug where neither the map not
                        // the player doesn't move
                        Game::tracking_player = false;
                        background.update_tracking();
                    }
                    break;
                case SDLK_l:
                    if (!Game::tracking_player){
                        // This condition is necessary (see SDLK_p comment)
                        Game::tracking_player = true;
                        background.update_tracking();
                    }
                    break;
                case SDLK_ESCAPE:
                    is_running = false;
                    break;
                default:
                    break;
            }
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
    label.getComponent<UILabel>().set_text(ss.str(), "andale");

    background.update();
    manager.update();

    // Sometimes we get a bouncing effect, despite still holding the button.
    // Though some time later we get back. We also get stuck sometimes...

    // I think the best thing is to do this predictively ? Looking ahead if we
    // are going to hit something. --> This is not the best if we have many objects
    // and very few collisions.

    /// TODO: This loop can definitely be optimized somehow.
    int nb_colliders = collider_vector.size();
    for (int i = 0; i < nb_colliders - 1; i++){          // From 0 to N-1
    for (int j = i+1; j < nb_colliders; j++){        // From I+1 to N
        Collider* CA = collider_vector[i]; Collider* CB = collider_vector[j];

        if (CA->handling_option + CB->handling_option == 0){
            // Small trick for checking if both colliders are immovable more quickly:
            // Instead of checking if each is IMMOVABLE_ON_COLLISION, we check if their sum is one,
            // since IMMOVABLE_ON_COLLISION is the enum's entry for 0.
            continue;
        }else if (CA->entity->has_group(PlayerGroup) && CB->entity->has_group(PlayerGroup)){
            // This is checked separately because it is less frequent than the previous one.
            continue;
        }
        // If we got here, then at least one of the objects is movable, let's check for
        // collision:
        bool collision_result = Collision::collider_AABB(CA, CB);
        if(collision_result){
            // We got a collision, let's handle it according to the collider options:
            std::size_t col_handle_A = CA->handling_option, col_handle_B = CB->handling_option;
            std::size_t proj_handle_A = CA->projectile_handling_option, proj_handle_B = CB->projectile_handling_option;

            /* To handle collision, we'll move the objects "back in time", until there was no
               collision. Looking into "Collision::collider_AABB", we'll see a series of logical
               cases that need to be met for a collision to take place. We want to move back in
               time to the limit case in which (at least) one of those conditions is not met. We
               can calculate that time in a straightforward way for each of the logical conditions.
               For the first one: 
               CA->transform->get_x() + CA->width  >= CB->transform->get_x() // we'll write as :
               XA + WA >= XB
               In the edge case where there's no collision:
               XA + WA = XB.
               Considering constant velocity (which is the current case in our implementation):
               X = VX * t + T0
               VXA * t + XA0 + WA = VXB * t + XB0
               t = (XA0 - XB0 + WA)/(VBX - VAX)
               t is the time in which this condition is not met. Since all four conditions have to
               be met for there to be a collision, we have to calculate this t for all four, and pick
               the earliest one.
               Note that we need to consider the opposite of the entity's velocities (because we are
               "going back in time"). XA0 and XB0 would be the current positions.
            */
            
            // For clarity:
            float xa = CA->transform->get_x();
            float ya = CA->transform->get_y();
            float xb = CB->transform->get_x();
            float yb = CB->transform->get_y();
            float speed_a = CA->transform->get_speed();
            float speed_b = CB->transform->get_speed();
            float vxa = CA->transform->get_vx();
            float vya = CA->transform->get_vy();
            float vxb = CB->transform->get_vx();
            float vyb = CB->transform->get_vy();
            float wa = CA->width;
            float ha = CA->height;
            float wb = CB->width;
            float hb = CB->height;

            float diff; // Amount of time to move.
            int t_idx;  // Information about collision axis.

            switch(col_handle_A){
            case (IMMOVABLE_ON_COLLISION):    // Stop B
                // // FIRST: CHECK PROJECTILE HANDLING BY A AND B.
                // if (CB->entity->has_group(ProjectileGroup)){
                // switch(proj_handle_A){
                // case(INDESTRUCTABLE_ON_PROJECTILE): break;  // Do nothing.
                // case(DESTROY_ON_PROJECTILE): CA->entity->destroy(); break;  // Destroy A
                // case(DAMAGE_ON_PROJECTILE):
                //     // TODO.
                //     break;
                // default:
                //     goto proj_error_case;
                // }}
                // // Now for B:
                // if (CA->entity->has_group(ProjectileGroup)){
                // switch(proj_handle_B){
                // case(INDESTRUCTABLE_ON_PROJECTILE): break;  // Do nothing.
                // case(DESTROY_ON_PROJECTILE): CB->entity->destroy(); break;  // Destroy A
                // case(DAMAGE_ON_PROJECTILE):
                //     // TODO.
                //     break;
                // default:
                //     goto proj_error_case;
                // }}

                speed_a = 0;    // Artificially set A's speed to 0 for calculations.
                switch (col_handle_B) {         
                // We exclude case(IMMOVABLE_ON_COLLISION) because it was already checked in line
                // (CA->handling_option + CB->handling_option == 0)
                case (MOVABLE_OBJECT):      // In both cases we'll stop B:
                case (PUSH_ON_COLLISION):
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update ONLY B's transform :
                    CB->transform->set_x( xb - speed_b*vxb* diff );
                    CB->transform->set_y( yb - speed_b*vyb* diff );
                    if (t_idx <= 1){ CB->transform->set_vx(0.0); }else{ CB->transform->set_vy(0.0); }
                    if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
                    if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
                    
                    break;
                case(DESTROY_ON_COLLISION):     // Destroy B
                    goto just_destroy_b;
                default:
                    goto error_case;
                }
                break;
            
            case(MOVABLE_OBJECT):
                switch (col_handle_B) {
                case (IMMOVABLE_ON_COLLISION):  // Stop A
                    goto stop_A;
                case (MOVABLE_OBJECT):          // Stop at the "middle".
                    goto stop_at_middle;
                case (PUSH_ON_COLLISION):       // Move B
                    /// TODO
                    break;
                case(DESTROY_ON_COLLISION):     // Destroy B
                    goto just_destroy_b;
                default:
                    goto error_case;
                }
                break;

            case(PUSH_ON_COLLISION):
                switch (col_handle_B) {
                case (IMMOVABLE_ON_COLLISION):    // Stop A
                stop_A:
                    speed_b = 0;
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update ONLY A's transform :
                    CA->transform->set_x( xa - speed_a*vxa* diff );
                    CA->transform->set_y( ya - speed_a*vya* diff );
                    if (t_idx <= 1){ CA->transform->set_vx(0.0); }else{ CA->transform->set_vy(0.0); }
                    if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
                    if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
                
                    break;
                case (MOVABLE_OBJECT):      // Move A
                    /// TODO
                    break;
                case (PUSH_ON_COLLISION):   // Stop at the "middle".
                stop_at_middle: // Regular time calculation, updating both entities:
                    std::tie(diff, t_idx) = Collision::get_collision_time(
                        xa, xb, ya, yb, speed_a, speed_b, vxa, vxb, vya, vyb, wa, wb, ha, hb);

                    // Update A:
                    CA->transform->set_x( xa - speed_a*vxa* diff );
                    CA->transform->set_y( ya - speed_a*vya* diff );
                    if (t_idx <= 1){ CA->transform->set_vx(0.0); }else{ CA->transform->set_vy(0.0); }
                    if (CA->entity->has_component<Sprite>()){ CA->entity->getComponent<Sprite>().update(); }
                    if (CA->entity->has_component<UILabel>()){ CA->entity->getComponent<UILabel>().update(); }
                
                    // Update B:
                    CB->transform->set_x( xb - speed_b*vxb* diff );
                    CB->transform->set_y( yb - speed_b*vyb* diff );
                    if (t_idx <= 1){ CB->transform->set_vx(0.0); }else{ CB->transform->set_vy(0.0); }
                    if (CB->entity->has_component<Sprite>()){ CB->entity->getComponent<Sprite>().update(); }
                    if (CB->entity->has_component<UILabel>()){ CB->entity->getComponent<UILabel>().update(); }
                
                    break;
                case(DESTROY_ON_COLLISION): // Destroy B
                    goto just_destroy_b;
                    break;
                default:
                    goto error_case;
                }
                break;

            case(DESTROY_ON_COLLISION): // Destroy A but:
                CA->entity->destroy();
                switch (col_handle_B) {
                case (DESTROY_ON_COLLISION):    // In this case, also destroy B.
                just_destroy_b:
                    CB->entity->destroy();
                    break;
                case (IMMOVABLE_ON_COLLISION): 
                    break;   
                case (MOVABLE_OBJECT):
                    break;
                case (PUSH_ON_COLLISION):
                    break;
                default:
                    goto error_case;
                }
                break;

            default:  // Unnexpected case: throw an error:
            error_case:
                std::cout << "Unknown case in collision detection. Options A and B: " \
                            << col_handle_A << "; " << col_handle_B << "." << std::endl;
                std::cout << "Entities: " << CA->entity->get_name() << "; " \
                            << CB->entity->get_name() << std::endl;
                throw std::runtime_error("Game.cpp: Game::update");
                break;
            }

            

        }
    }}
    /*
    for (Collider* cl : collider_vector){
        if (player.get_name() != cl->entity->get_name()){
            if(Collision::collider_AABB(player.getComponent<Collider>(), *cl)){
                // We've collided with something: take a step back:
                player.getComponent<Transform>().set_velocity(-1 * player.getComponent<Transform>().get_velocity());
                player.getComponent<Transform>().update();
                // Make the velocity zero:
                //player.getComponent<Transform>().set_vx(0.0);
                //player.getComponent<Transform>().set_vy(0.0);
                break;
            }
        }
    }*/

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


