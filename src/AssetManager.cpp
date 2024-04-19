// DEFINITION OF ASSETMANAGER CLASS IS IN GAME.HPP (AVOID CICULAR INCLUDES!)
#include <tuple>
#include "Game.hpp"

AssetManager::~AssetManager(){
    // Destroy all texture pointers.
    for (auto& t : texture_map){
        // Here we are looping through the pairs "t" of the map. We must pass
        // only the texture, so we use t.second. t.first would be the id.
        // Note that we must use auto& (and not auto) so that we actually alter
        // the entries of t.
        SDL_DestroyTexture(t.second);
    }
}

void AssetManager::add_texture(std::string id, std::string path){
    // Adds textures to the map members (texture_map, width_map, height_map).
    SDL_Texture* tmpTexture; int tmpW, tmpH;

    std::tie(tmpTexture, tmpW, tmpH) = AssetManager::load_texture(path);
    texture_map.emplace(id, tmpTexture);
    width_map.emplace(id, tmpW);
    height_map.emplace(id, tmpH);
}

SDL_Texture* AssetManager::get_texture(std::string id){
    return texture_map[id];
}

TexTup AssetManager::get_tuple(std::string id){
    return {texture_map[id], width_map[id], height_map[id]};
}



// From the old Texture Manager definition:
TexTup AssetManager::load_texture(std::string texture_file){
    int width, height;
    SDL_Surface* tmp_surface = SDL_LoadBMP(texture_file.c_str());
    width = tmp_surface->w;
    height = tmp_surface->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return {texture, width, height};
};


// Font Management:
void AssetManager::add_font(std::string id, std::string path, int font_size){
    font_map.emplace(id, TTF_OpenFont(path.c_str(), font_size));
}


TTF_Font* AssetManager::get_font(std::string id){
    return font_map[id];
}