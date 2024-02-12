#include "Sprite.hpp"
#include <stdexcept>    // Exception throwing

// DEFINITIONS:------------------------------------------------------------------------------
// Constructors:
Sprite::Sprite(std::string path,  bool is_animated):
    animated(false),
    frames(0),
    animation_period(0),
    nb_animations(0)
{
    animated = is_animated;
    set_texture(path);              // sets texture, image_width and image_height

    // Initializing source rect:
    src_rect.x = src_rect.y = 0;
    src_rect.w = image_width;
    src_rect.h = image_height;

    // Initializing destination rect:
    // Position will come in the init() method
    dst_rect.w = image_width;
    dst_rect.h = image_height;

};

// Base class methods: ------------------------------------------------
void Sprite::init(){
    // Definint destination_rect position from transform:
    transform = &entity->getComponent<Transform>();
    dst_rect.x = transform->get_x();
    dst_rect.y = transform->get_y();
}
void Sprite::update(){
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
        }
        src_rect.x = src_rect.w * static_cast<int>((SDL_GetTicks() / animation_period) % frames);
    }
    dst_rect.x = transform->get_x();
    dst_rect.y = transform->get_y();
}
void Sprite::render(){
    SDL_RenderCopyEx(Game::renderer, texture, &src_rect, &dst_rect, 0.0, NULL, sprite_flip);
}

// Texture and animation related: -------------------------------------
void Sprite::set_texture(std::string path){
    std::tie(texture, image_width, image_height) = TextureManager::load_texture_and_get_size(path);
};
void Sprite::add_animation(Animation animation, std::string animation_name){
    // Animations should be added in the order they appear in the image.
    // Error handling:
    if (!animated){
        std::cout << "Can't add animation to non-animated entity: " << entity->get_name();
        throw std::runtime_error("Can't add animation to non-animated entity");
    };
    // Increment nb_animations counter:
    nb_animations++;
    // Set the animation's index:
    animation.index = nb_animations;

    // Getting the source's height, based on the previous
    // animations added to the sprite component:
    std::map<std::string, Animation>::iterator it;
    for (it = animation_map.begin(); it != animation_map.end(); it++){
        if (it->second.index == nb_animations - 1){
            animation.src_height = it->second.src_height + it->second.sprite_height;
            break;
        }
    }
    // Checking for errors in getting source height:
    if ((animation.src_height == 0) && (nb_animations > 1)){
        throw std::runtime_error("Something went wrong when getting source height.");
    }

    // Add it to the map:
    animation_map.emplace(animation_name, animation);
}
void Sprite::set_animation(std::string animation_name){
    if(animated){
        current_animation = animation_name;
        // Set animation variables according to which animation to play:
        frames = animation_map[animation_name].frames;
        animation_period = animation_map[animation_name].animation_period;
        // Set source rectangle variables:
        src_rect.w = animation_map[animation_name].sprite_width;
        src_rect.h = animation_map[animation_name].sprite_height;
        src_rect.y = animation_map[animation_name].src_height;
        // Update destination_rects:
        dst_rect.w = scale_x * src_rect.w;
        dst_rect.h = scale_y * src_rect.h;
    }
}
// Overloading:
void Sprite::set_animation(int an_index){
    // Getting Animation name from index:
    std::string animation_name;
    std::map<std::string, Animation>::iterator it;
    for (it = animation_map.begin(); it != animation_map.end(); it++){
        if (it->second.index == an_index){
            animation_name = it->second.index;
            break;
        }
    }
    set_animation(animation_name);
}

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