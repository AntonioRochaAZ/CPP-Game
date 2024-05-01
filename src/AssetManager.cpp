// DEFINITION OF ASSETMANAGER CLASS IS IN GAME.HPP (AVOID CICULAR INCLUDES!)
#include <tuple>
#include "Game.hpp"
#include "utils.hpp"

// Image Management: ------------------------------------------------------------------------------
Game::AssetManager::~AssetManager(){
    // Destroy all texture pointers.
    for (auto& t : texture_map){
        // Here we are looping through the pairs "t" of the map. We must pass
        // only the texture, so we use t.second. t.first would be the id.
        // Note that we must use auto& (and not auto) so that we actually alter
        // the entries of t.
        SDL_DestroyTexture(t.second);
    }

    // Destroys all Font pointers:
    for (auto& f : font_map){
        TTF_CloseFont(f.second);
    }
}

void Game::AssetManager::add_texture(std::string id, std::string path){
    SDL_Surface* tmp_surface;   ///< Temporary surface (used to create texture).

    tmp_surface = SDL_LoadBMP(path.c_str());    // Load image as surface.
    width_map.emplace(id, tmp_surface->w);      // Save width/height into maps.
    height_map.emplace(id, tmp_surface->h);
    texture_map.emplace(id, SDL_CreateTextureFromSurface(Game::renderer, tmp_surface));      
        // Load texture and save it into map.
    SDL_FreeSurface(tmp_surface);               // Free surface.
}

SDL_Texture* Game::AssetManager::get_texture(std::string id){ 
    check_map_id<std::string, SDL_Texture*>(texture_map, id, "get_texture, texture_map");
    return texture_map[id];
}

TexTup Game::AssetManager::get_tuple(std::string id){
    check_map_id<std::string, SDL_Texture*>(texture_map, id, "get_tuple, texture_map");
    check_map_id<std::string, int>(width_map, id, "get_tuple, width_map");
    check_map_id<std::string, int>(height_map, id, "get_tuple, height_map");
    return {texture_map[id], width_map[id], height_map[id]};
}

void Game::AssetManager::destroy_texture(std::string id){
    check_map_id<std::string, SDL_Texture*>(texture_map, id, "destroy_texture, texture_map");
    SDL_DestroyTexture(texture_map[id]); texture_map.erase(id); }

// Font Management: -------------------------------------------------------------------------------
void Game::AssetManager::add_font(std::string id, std::string path, int font_size){
    font_map.emplace(id, TTF_OpenFont(path.c_str(), font_size)); }

TTF_Font* Game::AssetManager::get_font(std::string id){ 
    check_map_id<std::string, TTF_Font*>(font_map, id, "get_font, font_map");
    return font_map[id]; }

void Game::AssetManager::destroy_font(std::string id){
    check_map_id<std::string, TTF_Font*>(font_map, id, "destroy_font, font_map");
    TTF_CloseFont(font_map[id]); font_map.erase(id); }