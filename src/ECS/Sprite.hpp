#pragma once
// #include "Components.hpp"
#include "ECS.hpp"
#include "Transform.hpp"
#include "SDL2/SDL.h"
#include <cstdlib> // So we can halt execution
#include "Animation.hpp"
#include <map>
#include "../Game.hpp"
    // This last include is so that we can use asset manager's functions.
    // Might become deprecated in the future when we remove Sprite's possibility
    // to load textures, and only allow passing pre-loaded textures by an
    // asset manager.

class Sprite : public Component{
    private:    
        // Entity
        Transform* transform;               // Transform
        SDL_Texture* texture;               // Texture
        SDL_Rect src_rect, dst_rect;        // Rendering rectangles
        // Image related
        int image_width, image_height;      // Image shape
        int scale_x, scale_y;
        bool destroy_texture;

        // Animation:
        bool animated;                      // Whether it is animated
        int frames;                         // How many frames in the animation
        int animation_period;               // ms/frame
        int nb_animations;                  // Total number of animations.
        std::map<std::string, Animation> animation_map;    // Like a python dict.

    public:

        SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;

        // Constructors & destructors -----------------------------------------
        Sprite(std::string path, bool is_animated=false);   // Defined in Sprite.cpp file (long)
        Sprite(TexTup texture_tuple, bool is_animated=false);   // Defined in Sprite.cpp file (long)

        // Destructor:
        ~Sprite(){ if(destroy_texture){SDL_DestroyTexture(texture);} }

        // Base class methods: ------------------------------------------------
        void init() override;       // Defined in Sprite.cpp file (long)
        void update() override;     // Defined in Sprite.cpp file (long)
        void render() override;     // Defined in Sprite.cpp file (long)

        // Texture and animation related: -------------------------------------
        void add_animation(Animation animation, std::string animation_name);
        void set_animation(std::string animation_name);
        void set_animation(int an_index);   // Overloading
        std::string current_animation;      // Which animation to render
        int current_frame;                   // The frame of the animation we are at (starts at 0)

        // Size related -------------------------------------------------------
        // Getting and setting width and height:
        // NOTE: dst_rect's position is SET in the update method,
        // according to the Transform object. To change it,
        // one must change the Transform's position instead of
        // trying to change it here.
        int get_dst_width(){ return dst_rect.w; }       // Actual size on the screen
        int get_dst_height(){ return dst_rect.h; }
        void set_dst_width(int w){ dst_rect.w = w; }    // Actual size on the screen
        void set_dst_height(int h){ dst_rect.h = h; }
        int get_dst_x(){ return dst_rect.x; }
        int get_dst_y(){ return dst_rect.y; }
        // Setting SOURCE info:
        void set_src_width(int w){ src_rect.w = w; }    // Actual size on the screen
        void set_src_height(int h){ src_rect.h = h; }
        void set_src_x(int x){ src_rect.x = x; }    // Actual size on the screen
        void set_src_y(int y){ src_rect.y = y; }
        int get_src_x(){ return src_rect.x; }
        int get_src_y(){ return src_rect.y; }
        int get_src_width(){ return src_rect.w; }
        int get_src_height(){ return src_rect.h; }
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
