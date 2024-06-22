#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include <vector>
#include <map>
#include <string>
#include <eigen3/Eigen/Dense>
#include "ECS/ECS.hpp"
#include "SDL2/SDL_ttf.h"

/** This tuple is the return type of AssetManager::load_texture. It consists of the
SDL_Texture pointer, the texture's width and its height, respectively. */
using TexTup = std::tuple<SDL_Texture*, int, int>;

using vec = Eigen::Vector2f;

class Collider; // Forward declaration of the Collider class (otherwise, circular import).

/** Game class definition, used for running the game and handling events. Most functions are
defined in Game.cpp. */
class Game {
private:
    bool is_running;            ///< Whether the game is still running or it can be closed.
    int update_counter = 0;     ///< A tick counter (I believe it is deprecated TODO: REMOVE?)
    SDL_Window* window;         ///< The SDL window where the game is displayed.
    vec previous_camera_position = vec(0.0,0.0);    
        ///< Previous camera position (for updating).    
    Entity* camera_ref_entity;  ///< The entity we are following with the camera.  
    vec previous_ref_entity_position = vec(0.0,0.0);    
        ///< Previous position of the reference entity (for updating).  

public:
    Game();
    ~Game();

    int init(const char* title, int x, int y, int width, int height, bool fullscreen);
        ///< Initialization of SDL, the game screen and initial renderings.
    void handle_events();   ///< Handling inputs and events.
    void update();          ///< Go through our game objects and update them.
    void render();          ///< Render objects in the screen.
    void clean();           ///< Free memory.
    bool running(){return is_running;}; 
        ///< To tell if the game is still running. It is called in the main function at
        ///< each tick, to determine whether to finish execution or not.

    // static void add_tile(int x, int y, int w, int h, int id, std::string name);
    static SDL_Renderer* renderer;  ///< Renderer object responsible for rendering objects.
    static SDL_Event event;         ///< Event object used in event handling. 
    static std::vector<Collider*> collider_vector;  ///< Vector of collider objects for checking collision.
    
    static bool tracking_player;    ///< Whether the camera should follow the player or not.
    static vec camera_position;        
        ///< Where the camera is positioned (top-left of the screen).


    /** Definition of the AssetManager class, which manages textures and fonts.
    Function definitions are made in AssetManager.cpp. This class had to be defined in this file
    because it depends on the Game class definition, which also depends on AssetManager. Creating an
    AssetManager.hpp header would cause a circular import problem. 
    */
    class AssetManager{
    // private:
    public:

        /// Useful struct for structuring Animation information.
        struct Animation{
            int frames;             ///< Number of frames in the animation
            int animation_period;   ///< Period of each frame in miliseconds
            int sprite_width;       
            int sprite_height;
            
            // Members set up by the Sprite component:
            /** The animation index (for when the Entity has multiple animations).
                This member is set up by the \ref ::Sprite component when an animation
                is added through \ref Sprite::add_animation. 
            */
            int index;              
            /** The height (in pixels) at which we start "reading" the animation from the texture
                source file. This member is set up by the \ref ::Sprite component when an animation
                is added through \ref Sprite::add_animation.
            */
            int src_y;
            
            /** Constructor
                @param f: frames member.
                @param t: animation_period member (ms/frame).
                @param w: sprite_width member (pixels).
                @param h: sprite_height member (pixels).
            */
            Animation(int f, int t, int w, int h):
                frames(f), animation_period(t), sprite_width(w), sprite_height(h),
                index(-1), src_y(0){}
        };

        // I would like to use a shared pointer instead of a regular pointer here,
        // to try it out, but I am not sure how this would affect the destroy
        // texture function we must use to destroy textures. I'll use this then
        std::map< std::string, SDL_Texture* > texture_map;  ///< A map from strings (an ID) to textures objects.
        std::map< std::string, bool > is_animated_map;      ///< A map from strings (an ID) to a bool indicating if the texture has animations.
        std::map< std::string, 
            std::map< std::string, Animation >
            > animation_map;  ///< A map from strings (an ID) to a map of animation objects (animation name -> Animation object).
        std::map< std::string, int > width_map;  ///< A map from strings (an ID) to the associated texture's width.
        std::map< std::string, int > height_map; ///< A map from strings (an ID) to the associated texture's height.
        std::map< std::string, TTF_Font* > font_map; ///< A map from strings (an ID) to font objects.

        AssetManager(){};
        ~AssetManager();    ///< Destroys all texture and font pointers. Defined in AssetManager.cpp.

        // TEXTURE MANAGEMENT ---------------------------------------------------------------
        void add_texture(std::string id, std::string path); 
            ///< Load a texture from a path and save it under the ID "id" in the maps.
        void add_texture(std::string id, std::string path, 
                         std::map< std::string, Animation > sprite_animation_map); 
            ///< Load a texture from a path and save it under the ID "id" in the maps.
            ///< Then, save its animations in the animation map.
        SDL_Texture* get_texture(std::string id);  
            ///< Get a texture from the \ref AssetManager::texture_map "texture_map".
        TexTup get_tuple(std::string id);  
            ///< Get complete information from a texture (SDL_Texture* object, width and height).
        void destroy_texture(std::string id);
            ///< Destroy a texture based on its ID. This can be called when a given texture is
            ///< no longer used. This will also remove the map entry (necessary to avoid issues in
            ///< the destructor).

        // FONT MANAGEMENT ------------------------------------------------------------------
        void add_font(std::string id, std::string path, int font_size);
            ///< Add a new font to the \ref AssetManager::font_map "font_map", under the ID "id".
        TTF_Font* get_font(std::string id);
            ///< Get a font object from its ID.
        void destroy_font(std::string id);
            ///< Close a font based on its ID. This can be called when a given font is
            ///< no longer used. This will also remove the map entry (necessary to avoid issues in
            ///< the destructor).
    };

    static AssetManager assets;   
        ///< The game's AssetManager instance, which handles textures and fonts.
};

using Animation = Game::AssetManager::Animation;