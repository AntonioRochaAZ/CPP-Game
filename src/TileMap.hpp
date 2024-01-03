#pragma once
#include "Game.hpp"
#include <vector>

using map_shape = std::vector<std::vector<int>>;

class TileMap{
    public:
        TileMap();
        ~TileMap();

        static void load_map(std::string path, int x0, int y0, int w, int h);

        //void init(int wd_width, int wd_height, int tl_width, int tl_height);

        //void load_map(map_shape &map_array);
        //void render();

    private:

        static int window_width;
        static int window_height;
        
        static int tile_width;
        static int tile_height;

        static int nb_width_tiles;
        static int nb_height_tiles;

        // SDL_Rect src_rect, dst_rect;
        // Block types:
        std::vector<SDL_Texture*> type_texture;
        // SDL_Texture* stone; //0
        // SDL_Texture* grass; //1
        // SDL_Texture* water; //2

        // 2D integer array containing the 
        // map grid information:
        map_shape map;
};