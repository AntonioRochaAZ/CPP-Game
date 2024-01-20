#include "Sprite.hpp"
#include <stdexcept>    // Exception throwing

// DEFINITIONS:------------------------------------------------------------------------------
// Constructors:
// For static sprites:
Sprite::Sprite(std::string path):
    animated(false),         // TODO: change to false (here to debug.)
    frames(0),
    animation_period(1000)
{
    set_texture(path);              // sets texture, image_width and image_height
    dst_rect.w = image_width;
    dst_rect.h = image_height;
    src_rect.x = src_rect.y = 0;
    src_rect.h = image_height;
    src_rect.w = image_width;
};
// For animated sprites:
Sprite::Sprite(std::string path, int mFrames, int mPeriod){
    animated = true;
    frames = mFrames;
    animation_period = mPeriod; 
    set_texture(path);
    if (frames != 0){
        // There's no sense to use this constructor
        // otherwise, but still:
        int sprite_width  = image_width/frames;
        src_rect.w = sprite_width;
        dst_rect.w = sprite_width;
    }else{
        // Exception raise:
        throw std::invalid_argument("Warning: Animation constructor used but mFrames = 0 passed.");
        // std::cout << "Warning: Animation constructor used but mFrames = 0 passed.";
        // src_rect.w = image_width;
        // dst_rect.w = image_width;
    }
    src_rect.h = image_height;
    dst_rect.h = image_height;

    src_rect.x = src_rect.y = 0;
};


// Base class methods: ------------------------------------------------
void Sprite::init(){
    transform = &entity->getComponent<Transform>();
    dst_rect.x = transform->get_x();
    dst_rect.y = transform->get_y();
}
void Sprite::update(){
    // dst_rect.x = static_cast<int>(std::round(transform->x()));
    // dst_rect.y = static_cast<int>(std::round(transform->y()));
    // Works by itself.
    // TODO: decide which component will deal with the
    // scale (width, heigth)
    if (animated){
        // Update which frame we are using according to
        // in game time, animation speed and total number
        // of frames.
        if (frames == 0){
            std::cout << "\nERROR: Sprite is animated but frames is"
                << " set to zero in entity ";
            std::string some_name = std::string(entity->get_name());
            std::cout << some_name;
            throw std::runtime_error(" ");
            std::exit(1);
        }
        src_rect.x = src_rect.w * static_cast<int>((SDL_GetTicks() / animation_period) % frames);
    }
    dst_rect.x = transform->get_x();
    dst_rect.y = transform->get_y();
}
void Sprite::render(){
    SDL_RenderCopy(Game::renderer, texture, &src_rect, &dst_rect);
}

// Texture ------------------------------------------------------------
void Sprite::set_texture(std::string path){
    std::tie(texture, image_width, image_height) = TextureManager::load_texture_and_get_size(path);
};

// Size related -------------------------------------------------------
int Sprite::get_xscale(){
    if (image_width != 0){
        return (int)(dst_rect.w/image_width);
    }else{
        throw std::runtime_error("image_width = 0, while it shouldn't.");
    }
}
int Sprite::get_yscale(){
    if (image_height != 0){
        return (int)(dst_rect.h/image_height);
    }else{
        throw std::runtime_error("image_height = 0, while it shouldn't.");
    }
}