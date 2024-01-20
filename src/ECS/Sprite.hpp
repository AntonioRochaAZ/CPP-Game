#pragma once
// #include "Components.hpp"
#include "ECS.hpp"
#include "Transform.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"
#include <cstdlib> // So we can halt execution

class Sprite : public Component{
    private:    
        // Entity
        Transform* transform;               // Transform
        SDL_Texture* texture;               // Texture
        SDL_Rect src_rect, dst_rect;        // Rendering rectangles
        // Image
        int image_width, image_height;      // Image shape
        // Animation:
        bool animated;                      // Whether it is animated
        int frames;                         // How many frames in the animation
        int animation_period;               // ms/frame

    public:
        // Constructors & destructors -----------------------------------------
        // For static sprites:
        Sprite(std::string path);   // Defined in Sprite.cpp file (long)
        // For animated sprites:    // Defined in Sprite.cpp file (long)
        Sprite(std::string path, int mFrames, int mPeriod); 
        // Destructor:
        ~Sprite(){SDL_DestroyTexture(texture);}

        // Base class methods: ------------------------------------------------
        void init() override;       // Defined in Sprite.cpp file (long)
        void update() override;     // Defined in Sprite.cpp file (long)
        void render() override;     // Defined in Sprite.cpp file (long)

        // Texture ------------------------------------------------------------
        void set_texture(std::string path); // Defined in Sprite.cpp file (conciseness)

        // Size related -------------------------------------------------------
        // Getting and setting width and height:
        int get_width(){ return dst_rect.w; }       // Actual size on the screen
        int get_height(){ return dst_rect.h; }
        void set_width(int w){ dst_rect.w = w; }    // Actual size on the screen
        void set_height(int h){ dst_rect.h = h; }
        // Getting and setting scale:
        int get_xscale();   // Defined in Sprite.cpp file (exception handling)
        int get_yscale();   // Defined in Sprite.cpp file (exception handling)
        void set_xscale(float scale_x){dst_rect.w = scale_x * image_width;};
        void set_yscale(float scale_y){dst_rect.h = scale_y * image_height;};
        void set_scale(float scale){ 
            dst_rect.w = scale * image_width;
            dst_rect.h = scale * image_height;
        };
};
