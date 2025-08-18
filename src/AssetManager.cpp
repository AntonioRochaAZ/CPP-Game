// DEFINITION OF ASSETMANAGER CLASS IS IN GAME.HPP (AVOID CICULAR INCLUDES!)
#include <tuple>
#include <filesystem>
#include "Game.hpp"
#include "utils.hpp"
#include <fstream>
#include <sstream> // ostringstream definition

using Animation = Game::AssetManager::Animation;

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

    // Destroys all Mix_Chunk pointers:
    for (auto& audio : audio_map){
        Mix_FreeChunk(audio.second);
    }

    // Destroys all Mix_Music pointers:
    for (auto& music : music_map){
        Mix_FreeMusic(music.second);
    }

}

void Game::AssetManager::add_texture(std::string id, std::string path){
    // Variable declarations: -------------------------------------------------------------------------------
    SDL_Surface* tmp_surface;   ///< Temporary surface (used to create texture).
    std::size_t extension_idx;  ///< For getting the index of the "."
    std::string animation_path; ///< Path to the .txt file with the animation information
    //-------------------------------------------------------------------------------------------------------

    // Get texture:
    tmp_surface = SDL_LoadBMP(path.c_str());    // Load image as surface.
    width_map.emplace(id, tmp_surface->w);      // Save width/height into maps.
    height_map.emplace(id, tmp_surface->h);
    texture_map.emplace(id, SDL_CreateTextureFromSurface(Game::renderer, tmp_surface));      
        // Load texture and save it into map.
    SDL_FreeSurface(tmp_surface);               // Free surface.

    // Check if animation information is available:
    extension_idx = path.find_last_of(".");
    animation_path = path.substr(0, extension_idx) + "_animations.txt";
    if (!std::filesystem::exists(animation_path)){
        // No animation .txt file.
        is_animated_map.emplace(id, false);
    } else{
        // Then the file exists, let's get its information:
        // Variable declarations: ---------------------------------------------------------------------------
        std::ifstream animation_file;   ///< File object
        std::string line;               ///< Lines we'll parse.
        std::string token;              ///< The values we'll read from the line, as a string
        std::string animation_name;     ///< The animation name we'll read from the line.
        std::array<int, NB_ANIM_DATA> animation_data;  ///< Storage for the numerical values, which will be used to create the animation objects.
        std::map< std::string, Animation > temporary_map;   ///< Map storing all animations.
        int animation_index = 0;        ///< A counter which will be used to set the Animation objects up.
        int animation_y = 0;            ///< The value of y where the animation's texture starts (will be incremented).
        //---------------------------------------------------------------------------------------------------

        // Reading the animation text file:
        animation_file.open(animation_path);
        if (!animation_file.is_open()){ 
            std::cout << "ERROR: animation file exists, but could not be opened:";
            throw std::runtime_error(animation_path);
        }
        while (std::getline(animation_file, line)){ // getting line
            // Variable declarations: -----------------------------------------------------------------------
            std::stringstream ss(line); // A variable we can tokenize
            //-----------------------------------------------------------------------------------------------

            for (int i = 0; i < NB_ANIM_DATA + 1; i ++){   // Looping through data in the line
                if (!ss.good()){
                    throw std::runtime_error(
                        "Error in Game::AssetManager::add_texture: trying to read more values than there tokens/line in the text file.");
                }

                // Getting value:
                std::getline(ss, token, ',');
                switch (i){
                case 0:     // First one is the name of the animation
                    animation_name = token;
                    break;
                default:    // The rest are ints that will be stored in the array:
                    std::istringstream(token) >> animation_data[i - 1];
                    break;
                }
            }
            // Create animation object and store it to animation map
            Animation animation_object(animation_data[0], animation_data[1], animation_data[2], animation_data[3], animation_data[4], animation_index);
            animation_object.src_y = animation_y;           // Setting the source's y value up.
            
            animation_index++;                              // Increment animation index.
            animation_y += animation_object.sprite_height;  // Updating the source's height for the next animation.

            // animations added to the temporary map:
            temporary_map.emplace(animation_name, animation_object);
        }
        is_animated_map.emplace(id, true);
        animation_map.emplace(id, temporary_map);
    }
}

SDL_Texture* Game::AssetManager::get_texture(std::string id){ 
    #ifdef DEBUG_MODE
        check_map_id<std::string, SDL_Texture*>(texture_map, id, "get_texture, texture_map");
    #endif
    return texture_map[id];
}

TexTup Game::AssetManager::get_tuple(std::string id){
    #ifdef DEBUG_MODE
        check_map_id<std::string, SDL_Texture*>(texture_map, id, "get_tuple, texture_map");
        check_map_id<std::string, int>(width_map, id, "get_tuple, width_map");
        check_map_id<std::string, int>(height_map, id, "get_tuple, height_map");
    #endif
    return {texture_map[id], width_map[id], height_map[id]};
}

void Game::AssetManager::destroy_texture(std::string id){
    #ifdef DEBUG_MODE
        check_map_id<std::string, SDL_Texture*>(texture_map, id, "destroy_texture, texture_map");
    #endif
    SDL_DestroyTexture(texture_map[id]); 
    texture_map.erase(id); width_map.erase(id); height_map.erase(id);
}

// Font Management: -------------------------------------------------------------------------------
void Game::AssetManager::add_font(std::string id, std::string path, int font_size){
    font_map.emplace(id, TTF_OpenFont(path.c_str(), font_size)); }

TTF_Font* Game::AssetManager::get_font(std::string id){ 
    #ifdef DEBUG_MODE
        check_map_id<std::string, TTF_Font*>(font_map, id, "get_font, font_map");
    #endif
    return font_map[id]; }

void Game::AssetManager::destroy_font(std::string id){
    #ifdef DEBUG_MODE
        check_map_id<std::string, TTF_Font*>(font_map, id, "destroy_font, font_map");
    #endif
    TTF_CloseFont(font_map[id]); font_map.erase(id); }

int Game::AssetManager::get_text_center_position(
    std::string id, const char *text, int *w, int *h, int w0, int h0)
{
    #ifdef DEBUG_MODE
        check_map_id<std::string, TTF_Font*>(font_map, id, "get_text_size, font_map");
    #endif
    TTF_Font *font = get_font(id);
    int ret_val = TTF_SizeUTF8(font, text, w, h);
    std::cout << *w << " " << *h << std::endl;
    *w = (window_width  + w0 - *w)/2;
    *h = (window_height + h0 - *h)/2;
    return ret_val;
}

// Audio Management: ------------------------------------------------------------------------------
void Game::AssetManager::add_audio(std::string id, std::string path){
    Mix_Chunk* audio;
    audio = Mix_LoadWAV(path.c_str());
    if (audio == nullptr){ std::cout << "Error loading sound: " << path << std::endl; throw std::runtime_error(""); }
    audio_map.emplace(id, audio);
}

void Game::AssetManager::add_music(std::string id, std::string path){
    Mix_Music* music;
    music = Mix_LoadMUS(path.c_str());
    if (music == nullptr){ std::cout << "Error loading music: " << path << std::endl; throw std::runtime_error(""); }
    music_map.emplace(id, music);
}

Mix_Chunk* Game::AssetManager::get_audio(std::string id){
    #ifdef DEBUG_MODE
        check_map_id<std::string, Mix_Chunk*>(audio_map, id, "get_audio, audio_map");
    #endif
    return audio_map[id]; }

Mix_Music* Game::AssetManager::get_music(std::string id){
    #ifdef DEBUG_MODE
        check_map_id<std::string, Mix_Music*>(music_map, id, "get_music, music_map");
    #endif
    return music_map[id]; }