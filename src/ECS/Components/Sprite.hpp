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

class Sprite : public Component{
    public:

        SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;   
            /**< Whether to flip the texture. Will be dealt with by the 
                \ref KeyboardController Component, when the player turns
                around, for example.
             */

        // Constructors & destructors -----------------------------------------
        Sprite(TexTup texture_tuple, bool is_animated=false);   ///< Defined in Sprite.cpp file (long)
        // Destructor:
        ~Sprite(){}  

        // Base class methods: ------------------------------------------------
        void init() override;       ///< Defined in Sprite.cpp file (long)
        void update() override;     ///< Defined in Sprite.cpp file (long)
        void render() override;     ///< Defined in Sprite.cpp file (long)

        // Texture and animation related: -------------------------------------
        void add_animation(Animation animation, std::string animation_name);
            ///< Adds an animation to the animation_map member.
        void set_animation(std::string animation_name);
            ///< Changes current animation (reaches into the animation_map member).
        void set_animation(int an_index);   ///< Overloading.
        void set_texture(TexTup texture_tuple);  
            ///< Overriding the current texture and its w/h information. This is more for
            ///< showing off the different player models and is not supposed to remain later.
        std::string current_animation;      ///< Current animation being rendered.
        int current_frame;                  ///< The frame of the animation we are at (starts at 0).

        // Size related -------------------------------------------------------
        // Getting and setting width and height:
        // NOTE: dst_rect's position is SET in the update method,
        // according to the Transform object. To change it,
        // one must change the Transform's position instead of
        // trying to change it here.
        int get_dst_width(){ return dst_rect.w; }       ///< Gets actual width on the screen
        int get_dst_height(){ return dst_rect.h; }      ///< Gets actual height on the screen
        void set_dst_width(int w){ dst_rect.w = w; }    ///< Sets actual width on the screen
        void set_dst_height(int h){ dst_rect.h = h; }   ///< Sets actual height on the screen
        int get_dst_x(){ return dst_rect.x; }           ///< Gets X position on screen (pixels).
        int get_dst_y(){ return dst_rect.y; }           ///< Gets Y position on screen (pixels).
        // Setting SOURCE info:
        void set_src_width(int w){ src_rect.w = w; }    ///< Sets the width we want to get from the SOURCE texture.
        void set_src_height(int h){ src_rect.h = h; }   ///< Sets the height we want to get from the SOURCE texture.
        void set_src_x(int x){ src_rect.x = x; }        ///< Sets the X position in the SOURCE texture.
        void set_src_y(int y){ src_rect.y = y; }        ///< Sets the Y position in the SOURCE texture.
        int get_src_x(){ return src_rect.x; }           ///< Gets the X position from the SOURCE texture.
        int get_src_y(){ return src_rect.y; }           ///< Gets the Y position from the SOURCE texture.
        int get_src_width(){ return src_rect.w; }       ///< Gets the width we are using for the SOURCE texture.
        int get_src_height(){ return src_rect.h; }      ///< Gets the height we are using for the SOURCE texture.
        // Getting and setting scale:
        int get_xscale();   ///< Defined in Sprite.cpp file (exception handling)
        int get_yscale();   ///< Defined in Sprite.cpp file (exception handling)
        void set_xscale(float sclx){
            scale_x = sclx; 
            dst_rect.w = scale_x * src_rect.w;
        };
        void set_yscale(float scly){
            scale_y = scly;
            dst_rect.h = scale_y * src_rect.h;
        };
        void set_scale(float scale){ set_xscale(scale); set_yscale(scale); };  ///< Sets the same scale for both x and y axes.

    private:    
        // Entity
        Transform* transform;               ///< Entity's \ref ::Transform component.
        SDL_Texture* texture;               ///< Pointer to the Texture used.
        SDL_Rect src_rect, dst_rect;        ///< Rendering rectangles (source and destination).
        // Image related
        int image_width, image_height;      ///< Image shape (source).
        int scale_x, scale_y;               
            /**< Scale of the rendering: size on screen/size of the source
                (``scale_x = dst_rect.w/src_rect.w`` and ``scale_y = dst_rect.h/src_rect.h``).
            */

        // Animation related:
        bool animated;                      ///< Whether the sprite is is animated.
        int frames;                         ///< How many frames in the current animation.
        int animation_period;               ///< The period of the current animation being played (ms/frame).
        int nb_animations;                  ///< Total number of animations.
        std::map<std::string, Animation> animation_map;
            ///< Map translating animation names to animation objects.
};
