#pragma once
#include "Game.hpp"

class TextureManager{
    public:
        static SDL_Texture* load_texture(const char* texture_file);
        static std::tuple<SDL_Texture*, int, int> load_texture_and_get_size(const char* texture_file);
        static void draw(SDL_Texture* texture_object, SDL_Rect src, SDL_Rect dst);
};

