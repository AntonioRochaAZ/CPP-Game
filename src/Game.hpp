#pragma once

#include <iostream>
#include "SDL.h"
#include <vector>
#include <map>
#include <string>
#include <Eigen/Dense>
#include "ECS/ECS.hpp"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

/** This tuple is the return type of AssetManager::load_texture. It consists of the
SDL_Texture pointer, the texture's width and its height, respectively. */
using TexTup = std::tuple<SDL_Texture*, int, int>;

using vec = Eigen::Vector2f;

struct Collider; // Forward declaration of the Collider class (otherwise, circular import).

constexpr int NB_ANIM_DATA = 5; // Number of numeric animation data (thus excluding animation name) stored in _animation.txt files 

// The following window information is defined in Game.cpp
extern const float window_aspect_ratio;
extern const int  initial_window_height;
extern const int  initial_window_width;
extern const bool fullscreen;

/** Game class definition, used for running the game and handling events. Most functions are
defined in Game.cpp. */
class Game {
private:
    bool is_running;            ///< Whether the game is still running or it can be closed.
    int update_counter = 0;     ///< A tick counter (I believe it is deprecated TODO: REMOVE?)
    vec previous_camera_position = vec(0.0,0.0);    
        ///< Previous camera position (for updating).    
    std::weak_ptr<Entity> camera_ref_entity; 
        ///< The entity we are following with the camera.  
    vec previous_ref_entity_position = vec(0.0,0.0);    
        ///< Previous position of the reference entity (for updating).  
    void update_camera_ref_entity();
        ///< For dealing with the camera reference entity if it is destroyed.

public:
    Game();
    ~Game();

    int init(const char* title);
        ///< Initialization of SDL, the game screen and initial renderings.
    void handle_events();   ///< Handling inputs and events.
    void update();          ///< Go through our game objects and update them.
    void render();          ///< Render objects in the screen.
    void clean();           ///< Free memory.
    bool running(){return is_running;}; 
        ///< To tell if the game is still running. It is called in the main function at
        ///< each tick, to determine whether to finish execution or not.

    // static void add_tile(int x, int y, int w, int h, int id, std::string name);
    static SDL_Window* m_window;         ///< The SDL window where the game is displayed.
    static SDL_Renderer* renderer;  ///< Renderer object responsible for rendering objects.
    static SDL_Event event;         ///< Event object used in event handling. 
    static std::vector<Collider*> collider_vector;  ///< Vector of collider objects for checking collision.
    
    static bool tracking_player;    ///< Whether the camera should follow the player or not.
    static vec camera_position;        
        ///< Where the camera is positioned (top-left of the screen).
    static std::shared_ptr<Entity> cursor;


    /** Definition of the AssetManager class, which manages textures and fonts.
    Function definitions are made in AssetManager.cpp. This class had to be defined in this file
    because it depends on the Game class definition, which also depends on AssetManager. Creating an
    AssetManager.hpp header would cause a circular import problem. 
    */
    struct AssetManager{

        /// Useful struct for structuring Animation information.
        struct Animation{
            int frames;             ///< Number of frames in the animation
            int animation_period;   ///< Period of each frame in miliseconds
            int sprite_width;       
            int sprite_height;
            int m_flip_shift;       ///< By how many pixels to shift (to the left) the sprite when when Sprite's sprite_flip is SDL_FLIP_HORIZONTAL.
            int index;              ///< The animation index (for when the Entity has multiple animations).

            // Members set up when loading the animations:
            /** The height (in pixels) at which we start "reading" the animation from the texture
                source file. This member is set up by the \ref ::AssetManager::add_texture member
                when an animation is added
            */
            int src_y;
            
            /** Constructor
                @param f: frames member.
                @param t: animation_period member (ms/frame).
                @param w: sprite_width member (pixels).
                @param h: sprite_height member (pixels).
                @param flip_shift: The number of pixels to shift the sprite to the left when its sprite_flip member is SDL_FLIP_HORIZONTAL (pixels).
                @param i: the index of the animation.
            */
            Animation(int f, int t, int w, int h, int flip_shift, int i):
                frames(f), animation_period(t), sprite_width(w), sprite_height(h),
                m_flip_shift(flip_shift), index(i), src_y(0){}
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
        std::map< std::string, Mix_Chunk* > audio_map; ///< A map from strings (an ID) to SDL_Mixer Mix_Chunk pointers.
        std::map< std::string, Mix_Music* > music_map; ///< A map from strings (an ID) to SDL_MIxer Mix_Music pointersr.

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
        int get_text_center_position(std::string id, const char *text, int *w, int *h, int w0=0, int h0=0);

        // AUDIO MANAGEMENT ------------------------------------------------------------------
        SDL_AudioDeviceID m_audio_device;
        SDL_AudioSpec m_obtained_audio_spec;
        void add_audio(std::string id, std::string path);
        void add_music(std::string id, std::string path);
        Mix_Chunk* get_audio(std::string id);
        Mix_Music* get_music(std::string id);

    };

    static AssetManager assets;   
        ///< The game's AssetManager instance, which handles textures and fonts.

    static int audio_volume;
    static int music_volume;
};

using Animation = Game::AssetManager::Animation;