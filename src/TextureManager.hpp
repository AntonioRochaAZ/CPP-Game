#pragma once
#include "Game.hpp"

class TextureManager{
    public:
        static SDL_Texture* load_texture(std::string texture_file);
        static std::tuple<SDL_Texture*, int, int> load_texture_and_get_size(std::string texture_file);
        static void draw(
            SDL_Texture* texture_object, SDL_Rect src, SDL_Rect dst,
            SDL_RendererFlip flip = SDL_FLIP_NONE
            );
};

