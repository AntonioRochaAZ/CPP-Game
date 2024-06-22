#include "Sprite.hpp"
#include <stdexcept>    // Exception throwing
#include "../../utils.hpp"  // map id checking.
#include "Collider.hpp"  // map id checking.

// DEFINITIONS:------------------------------------------------------------------------------
// Constructors:
// Sprite::Sprite(std::string texture_id):
//     frames(0),
//     animation_period(0),
//     nb_animations(0)
// {
//     check_map_id<std::string, SDL_Texture*>(
//         Game::assets.texture_map, texture_id, "Sprite::Sprite, Game::assets.texture_map");

//     // get texture tuple from Asset Manager,
//     // Check (with halt_bool = false) if there are animations in Game::assets.animation_map 
//     // for the given texture_id. If so, animated = true, get the number of animations (and other
//     // related Sprite members), and we can also set the animation_map directly.
//     std::tie(texture, image_width, image_height) = texture_tuple;

//     // Initializing source rect:
//     src_rect.x = src_rect.y = 0;
//     src_rect.w = image_width;
//     src_rect.h = image_height;

//     // Initializing destination rect:
//     // Position will come in the init() method
//     dst_rect.w = image_width;
//     dst_rect.h = image_height;

//     // Since we have passed the actual Texture pointer, we mustn't
//     // destroy the texture in the destructor.
//     // (Now initialized before-hand):
//     //destroy_texture = false;
// };

Sprite::Sprite(TexTup texture_tuple, bool is_animated):
    animated(is_animated),
    frames(0),
    animation_period(0),
    nb_animations(0)
{
    std::tie(texture, image_width, image_height) = texture_tuple;

    // Initializing source rect:
    src_rect.x = src_rect.y = 0;
    src_rect.w = image_width;
    src_rect.h = image_height;

    // Initializing destination rect:
    // Position will come in the init() method
    dst_rect.w = image_width;
    dst_rect.h = image_height;

    // Since we have passed the actual Texture pointer, we mustn't
    // destroy the texture in the destructor.
    // (Now initialized before-hand):
    //destroy_texture = false;
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
        current_frame = static_cast<int>((SDL_GetTicks() / animation_period) % frames);
        src_rect.x = src_rect.w * current_frame;
    }
    dst_rect.x = transform->get_x();
    dst_rect.y = transform->get_y();
}
void Sprite::render(){
    dst_rect.x -= Game::camera_position[0]; // Updating position according to camera.
    dst_rect.y -= Game::camera_position[1];
    SDL_RenderCopyEx(Game::renderer, texture, &src_rect, &dst_rect, 0.0, NULL, sprite_flip);
}

// Texture and animation related: -------------------------------------
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
            animation.src_y = it->second.src_y + it->second.sprite_height;
            break;
        }
    }
    // Checking for errors in getting source height:
    if ((animation.src_y == 0) && (nb_animations > 1)){
        throw std::runtime_error("Something went wrong when getting source height.");
    }

    // Add it to the map:
    animation_map.emplace(animation_name, animation);
}

void Sprite::set_animation(std::string animation_name){
    if(animated){
        check_map_id<std::string, Animation>(animation_map, animation_name, "set_animation, animation_map");
        
        Animation& local_animation = animation_map.find(animation_name)->second;
            ///< Here we pass by the iterator to avoid issues with not having a default
            ///< constructor for the Animation class.
        current_animation = animation_name;
        // Set animation variables according to which animation to play:
        frames = local_animation.frames;
        animation_period = local_animation.animation_period;
        // Set source rectangle variables:
        src_rect.w = local_animation.sprite_width;
        src_rect.h = local_animation.sprite_height;
        src_rect.y = local_animation.src_y;
        // Update destination_rects:
        dst_rect.w = scale_x * src_rect.w;
        dst_rect.h = scale_y * src_rect.h;
    }
    if (entity->has_component<Collider>()){
        if (entity->getComponent<Collider>().dynamic_shape){
            entity->getComponent<Collider>().set_width( dst_rect.w);
            entity->getComponent<Collider>().set_height(dst_rect.h);
        }
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

void Sprite::set_texture(TexTup texture_tuple){
    std::tie(texture, image_width, image_height) = texture_tuple;
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