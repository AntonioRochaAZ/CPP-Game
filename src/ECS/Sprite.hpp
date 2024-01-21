#pragma once
// #include "Components.hpp"
#include "ECS.hpp"
#include "Transform.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"
#include <cstdlib> // So we can halt execution
#include "Animation.hpp"
#include <map>

class Sprite : public Component{
    private:    
        // Entity
        Transform* transform;               // Transform
        SDL_Texture* texture;               // Texture
        SDL_Rect src_rect, dst_rect;        // Rendering rectangles
        // Image related
        int image_width, image_height;      // Image shape
        int scale_x, scale_y;
        // Animation:
        bool animated;                      // Whether it is animated
        int frames;                         // How many frames in the animation
        int animation_period;               // ms/frame
        int nb_animations;                  // Total number of animations.
        std::string current_animation;      // Which animation to render.
        std::map<std::string, Animation> animation_map;    // Like a python dict.

    public:

        SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;

        // Constructors & destructors -----------------------------------------
        Sprite(std::string path, bool is_animated=false);   // Defined in Sprite.cpp file (long)
        // Destructor:
        ~Sprite(){SDL_DestroyTexture(texture);}

        // Base class methods: ------------------------------------------------
        void init() override;       // Defined in Sprite.cpp file (long)
        void update() override;     // Defined in Sprite.cpp file (long)
        void render() override;     // Defined in Sprite.cpp file (long)

        // Texture and animation related: -------------------------------------
        void set_texture(std::string path); // Defined in Sprite.cpp file (conciseness)
        void add_animation(Animation animation, std::string animation_name);
        void set_animation(std::string animation_name);
        void set_animation(int an_index);   // Overloading

        // Size related -------------------------------------------------------
        // Getting and setting width and height:
        int get_width(){ return dst_rect.w; }       // Actual size on the screen
        int get_height(){ return dst_rect.h; }
        void set_width(int w){ dst_rect.w = w; }    // Actual size on the screen
        void set_height(int h){ dst_rect.h = h; }
        // Getting and setting scale:
        int get_xscale();   // Defined in Sprite.cpp file (exception handling)
        int get_yscale();   // Defined in Sprite.cpp file (exception handling)
        void set_xscale(float sclx){
            scale_x = sclx; 
            dst_rect.w = scale_x * src_rect.w;
        };
        void set_yscale(float scly){
            scale_y = scly;
            dst_rect.h = scale_y * src_rect.h;
        };
        void set_scale(float scale){
            set_xscale(scale); set_yscale(scale);
        };
};
