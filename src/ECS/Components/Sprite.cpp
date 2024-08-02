#include "Sprite.hpp"
#include <stdexcept>    // Exception throwing
#include "../../utils.hpp"  // map id checking.
#include "Collider.hpp"  // map id checking.

// DEFINITIONS:------------------------------------------------------------------------------
// Constructors:
Sprite::Sprite(std::string texture_id): set_collider(false), m_texture_id(texture_id){
    set_texture(texture_id);
};

/** This constructor is kept for the TileMap, for which multiple tile textures
are contained in one single image file and thus texture object. This allows
manual control over the sprite.
*/
Sprite::Sprite(TexTup texture_tuple, bool is_animated):
    set_collider(false),
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
/** 
    Defining destination_rect position from transform:
    Note that the transform will always have already been defined because
    it MUST be updated before the sprite, so there is no reason to
    check if it exists here.
*/
void Sprite::init(){
    transform = &entity->getComponent<Transform>();
    dst_rect.x = transform->x;
    dst_rect.y = transform->y;
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
        current_frame = static_cast<int>(((SDL_GetTicks64() - m_reference_time)/ animation_period) % frames);
        src_rect.x = src_rect.w * current_frame;
    }
    dst_rect.x = transform->x;
    dst_rect.y = transform->y;
}

void Sprite::render(){
    dst_rect.x -= Game::camera_position[0]; // Updating position according to camera.
    dst_rect.y -= Game::camera_position[1];
    SDL_RenderCopyEx(Game::renderer, texture, &src_rect, &dst_rect, 0.0, NULL, sprite_flip);
}

// Texture and animation related: -------------------------------------
/** Although dealing with animations is mostly automatized now, this manual control
could be useful if we would like to manually tune some things, especially if we want
to have animated tiles, for example. Should be avoided, though. 
*/
void Sprite::add_animation(Animation animation, std::string animation_name){
    // Animations should be added in the order they appear in the image.
    
#ifdef DEBUG_MODE
    // Error handling:
    if (!animated){
        std::cout << "Can't add animation to non-animated entity: " << entity->get_name();
        throw std::runtime_error("Can't add animation to non-animated entity");
    };
#endif

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
    
    // Let's first check if the passed animation isn't
    // the animation that is already set (if so, return):
    if (current_animation == animation_name){
        return;
    }

#ifdef DEBUG_MODE
    // Checking if the object is animated:
    if (!animated){
        std::cout << "Tried calling 'set_animation' when sprite isn't animated. Entity: " 
                  << entity->name << "Animation name: " << animation_name << std::endl;
        throw std::runtime_error(entity->name);
    }else{
        // If it is, checking if the animation is in the animation_map
        check_map_id<std::string, Animation>(
            animation_map, animation_name, "set_animation, animation_map");
    }
#endif
    
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
    // Update reference time:
    m_reference_time = SDL_GetTicks64();
    maybe_update_collider();
}
// Overloading:
void Sprite::set_animation(int an_index){
    // Getting Animation name from index:
    std::string animation_name = "";
    std::map<std::string, Animation>::iterator it;
    for (it = animation_map.begin(); it != animation_map.end(); it++){
        if (it->second.index == an_index){
            animation_name = it->first;
            break;
        }
    }
    if (animation_name == ""){
        std::cout << "This animation is not here! Entity: " << entity->name << "; animation index: " << an_index << std::endl;
        throw std::runtime_error(entity->name);
    }
    set_animation(animation_name);
}

void Sprite::set_texture(std::string texture_id){
    // Get texture tuple from Asset Manager.
    std::tie(texture, image_width, image_height) = Game::assets.get_tuple(texture_id);

    // Get animation map
    src_rect.x = 0;
    if (Game::assets.is_animated_map[texture_id]){
        animated = true;
        animation_map = Game::assets.animation_map[texture_id]; 
            // We want this to be a copy, so we can eventually touch one sprite's animations
            // without changeing all textures that use the same ones!
        nb_animations = animation_map.size();
        set_animation(0);   
            // Set the current animation as the first one. This will setup some members
    } else{
        animated = false; frames = 0; animation_period = 0; nb_animations = 0;

        // Initializing source rect:
        src_rect.y = 0;
        src_rect.w = image_width;
        src_rect.h = image_height;

        // Initializing destination rect:
        // Position will come in the init() method
        dst_rect.w = image_width;
        dst_rect.h = image_height;
        maybe_update_collider();
    }
}

// Size related: --------------------------------------------------------------
/**
    Checks if set_collider is true and updates the collider's shape if so.
    
    Why not check for a member of collider? 
    Like getComponent<Collider>().dynamic_shape ?
    Because this function is called in Sprite's initialization, before
    Sprite's "entity" member is initialized. So we can't access other 
    components yet... It is easier to initialize set_collider to false
    and have this called.
*/
void Sprite::maybe_update_collider(){
    if(set_collider){ entity->getComponent<Collider>().get_shape(); }
}