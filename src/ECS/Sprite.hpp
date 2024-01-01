#pragma once
#include "Components.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"

class Sprite : public Component{
    private:
        Transform* transform;
        SDL_Texture* texture;
        SDL_Rect src_rect, dst_rect;

        int sprite_width, sprite_height;

    public:
        Sprite() = default;
        Sprite(const char* path){
            set_texture(path);
        }
        void set_texture(const char* path){
            std::tie(texture, sprite_width, sprite_height) = TextureManager::load_texture_and_get_size(path);
        }
        int get_width(){ return dst_rect.w; } // Actual size on the screen
        int get_height(){ return dst_rect.h; }
        void init() override{
            transform = &entity->getComponent<Transform>();

            src_rect.x = src_rect.y = 0;
            src_rect.w = sprite_width;
            src_rect.h = sprite_height;
            transform->set_width(sprite_width);
            transform->set_height(sprite_width);
            // TODO: add options to change these later:
            dst_rect.w = sprite_width;
            dst_rect.h = sprite_height;
        }
        void update() override{
            // dst_rect.x = static_cast<int>(std::round(transform->x()));
            // dst_rect.y = static_cast<int>(std::round(transform->y()));
            // Works by itself.
            // TODO: decide which component will deal with the
            // scale (width, heigth)
            dst_rect.x = transform->x();
            dst_rect.y = transform->y();
            dst_rect.w = transform->w();
            dst_rect.h = transform->h();
        }
        void render() override{
            SDL_RenderCopy(Game::renderer, texture, &src_rect, &dst_rect);
        }
        ~Sprite(){SDL_DestroyTexture(texture);}
};