#pragma once
// #include "Components.hpp"
#include "../ECS.hpp"
#include "Transform.hpp"
#include "SDL2/SDL.h"
#include <cstdlib> // So we can halt execution
#include <map>
#include "../../Game.hpp"
    // This last include is necessary to use the TexTup typedef,
    // and will be used in Sprite.cpp when we call Game::renderer.


/** 
    NOTE: dst_rect's position is SET in the update method,
    according to the Transform object. To change it,
    one must change the Transform's position instead of
    trying to change it here. 
*/
class Sprite : public Component{
private:
    // These members are private because, when updated, we may need to
    // updated the Collider's shape. This means we have to systematically
    // call the maybe_update_collider() function. To void issues where the
    // shape of the sprite is changed and the collider shape isn't updated,
    // we force these to be private.
    SDL_Rect dst_rect;      ///< Rendering rectangle (source).
    int scale_x, scale_y;   ///< Scale factors for (size on screen)/(source size).     

public:
    // Members: ---------------------------------------------------------------
    Transform* transform;   ///< Entity's \ref ::Transform component.
    bool set_collider;      ///< Whether to set \ref ::Collider dimensions automatically when changing animations.          
       
    // Image related (source):
        int image_width, image_height;      ///< Image shape (source).
    
    // Texture related (on screen):
        std::string m_texture_id;           ///< The texture ID.
        SDL_Texture* texture;               ///< Pointer to the Texture used.
            ///< (``scale_x = dst_rect.w/src_rect.w`` and ``scale_y = dst_rect.h/src_rect.h``).
        SDL_Rect src_rect;                  ///< Rendering rectangle (source).   
        SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;   
            ///< Whether to flip the texture. Will be dealt with by the 
            ///< \ref KeyboardController Component, when the player turns
            ///< around, for example.
    
    // Animation related:
        bool animated;                      ///< Whether the sprite is is animated.
        int frames;                         ///< How many frames in the current animation.
        int animation_period;               ///< The period of the current animation being played (ms/frame).
        int nb_animations;                  ///< Total number of animations.
        std::map<std::string, Animation> animation_map; ///< Map translating animation names to animation objects.
        std::string current_animation;      ///< Current animation being rendered.
        int current_frame;                  ///< The frame of the animation we are at (starts at 0).
        Uint64 m_reference_time;            ///< Reference time for calculating which frame to show.
    //<------------------------------------------------------------------------
    

    // Constructors: ----------------------------------------------------------
    // Implemented and documented in Sprite.cpp.
    Sprite(std::string texture_id);   
    Sprite(TexTup texture_tuple, bool is_animated=false);  

    // Base class methods: ----------------------------------------------------
    void init() override;       ///< Defined in Sprite.cpp file (long)
    void update() override;     ///< Defined in Sprite.cpp file (long)
    void render() override;     ///< Defined in Sprite.cpp file (long)

    // Texture and animation related: -----------------------------------------
    void add_animation(Animation animation, std::string animation_name);    // Implemented and documented in Sprite.cpp.
    void set_animation(std::string animation_name); ///< Changes current animation (reaches into the animation_map member).
    void set_animation(int an_index);   ///< Overloading.
    void set_texture(std::string texture_id);  
        ///< Overriding the current texture and its width/height information. This is more for
        ///< showing off the different player models and is not supposed to remain later.

    // Size related: ----------------------------------------------------------
    // THESE FUNCTIONS EXIST BECAUSE FOR CALLING maybe_update_collider() 
    // AUTOMATICALLY.
    void maybe_update_collider();   // Implemented and documented in Sprite.cpp

    // Getter/Setter functions:
    // They must be used so that maybe_update_collider is systematically called.
    // No get_xscale/ get_yscale functions because not needed for the moment.
        void set_dst_width(int w){ dst_rect.w = w; maybe_update_collider();}    ///< Sets actual width on the screen
        void set_dst_height(int h){ dst_rect.h = h; maybe_update_collider(); }  ///< Sets actual height on the screen
        int get_dst_width(){ return dst_rect.w; }    ///< Gets actual width on the screen
        int get_dst_height(){ return dst_rect.h; }  ///< Gets actual height on the screen
        void set_xscale(float sclx){ scale_x = sclx; set_dst_width(sclx * src_rect.w); };
        float get_xscale(){ return scale_x; };
        void set_yscale(float scly){ scale_y = scly; set_dst_height(scly * src_rect.h); };
        float get_yscale(){ return scale_y; };
        void set_scale(float scale){ set_xscale(scale); set_yscale(scale); }; 
            ///< Sets the same scale for both x and y axes.
    
};
