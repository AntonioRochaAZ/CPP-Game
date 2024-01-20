#include <tuple>
#include "TextureManager.hpp"

SDL_Texture* TextureManager::load_texture(std::string texture_file){

    SDL_Surface* tmp_surface = SDL_LoadBMP(texture_file.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return texture;
};

std::tuple<SDL_Texture*, int, int> TextureManager::load_texture_and_get_size(std::string texture_file){
    int width, height;
    SDL_Surface* tmp_surface = SDL_LoadBMP(texture_file.c_str());
    width = tmp_surface->w;
    height = tmp_surface->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return {texture, width, height};
};

void TextureManager::draw(SDL_Texture* texture_object, SDL_Rect src, SDL_Rect dst){
    SDL_RenderCopy(Game::renderer, texture_object, &src, &dst);
}