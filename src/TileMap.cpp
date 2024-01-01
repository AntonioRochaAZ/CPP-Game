#include "TileMap.hpp"
#include "TextureManager.hpp"
#include <vector>
#include <cmath>
#include "Game.hpp"
#include <fstream> // Reading files

using map_shape = std::vector< std::vector<int> >;

TileMap::TileMap(){
    type_texture = {
        TextureManager::load_texture("assets/stone0.bmp"),   // 0
        TextureManager::load_texture("assets/stone1.bmp")   // 0
    };
}

void TileMap::load_map(std::string path, int x0, int y0, int w, int h){
    char tile;
    std::ifstream map_file;
    int tile_width = 100;
    int tile_height = 100;
    map_file.open(path);
        for (int y=0; y<h; y++){
            for (int x=0; x<w; x++){
                map_file.get(tile);     // A number.
                // atoi turns a string into an int.
                // std::cout << "Tile " << y*10 + x << ": " << tile;
                Game::add_tile(x0 + x*tile_width, y0 + y*tile_height, tile_width, tile_height, atoi(&tile));
                map_file.ignore();      // A comma.
            }
            std::cout << "\n";
        }
    map_file.close();
}

/*
void TileMap::init(int wd_width, int wd_height, int tl_width, int tl_height){
    // Saving size information:
    window_width = wd_width;
    window_height = wd_height;
    tile_width = tl_width;
    tile_height = tl_height;
    // Calculating the number of tiles:
    nb_width_tiles = ceil(window_width/tile_width);
    nb_height_tiles = ceil(window_height/tile_height);
    // Initializing the map 2D vector with 0s
    map = std::vector<std::vector<int>>(nb_height_tiles, std::vector<int>(nb_width_tiles, 0));
    // for (int row = 0; row < nb_height_tiles; row ++){
    //     for (int column = 0; column < nb_width_tiles; column ++){
    //         std::cout << map[row][column] << ", ";
    //     }
    //     std::cout << "\n";
    // }
    // Define source and destination rectangles:
    src_rect.w = dst_rect.w = tl_width;
    src_rect.h = dst_rect.h = tl_height;
    // These next ones will be changed dynamically when drawing:
    src_rect.x = src_rect.y = 0;
    dst_rect.x = dst_rect.y = 0;
};

void TileMap::load_map(map_shape &map_array){
    for (int row = 0; row < nb_height_tiles; row++){
        for (int column = 0; row < nb_width_tiles; column++){
            map[row][column] = map_array[row][column];
        };
    };
};

void TileMap::render(){
    int type;
    for (int row = 0; row < nb_height_tiles; row++){
        for (int column = 0; column < nb_width_tiles; column++){
            type = map[row][column];
            dst_rect.x = column*tile_width;
            dst_rect.y = row*tile_height;
            TextureManager::draw(type_texture[ type ], src_rect, dst_rect);
        };
    };
};

*/

TileMap::~TileMap(){
    // If it does not work, turn the type_texture vector into a
    // vector of texture pointers?
    for (auto& tex : type_texture) SDL_DestroyTexture(tex);
}