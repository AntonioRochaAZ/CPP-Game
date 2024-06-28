#pragma once
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Game.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include <stdexcept>    // Exception throwing

using map_shape = std::vector< std::vector<int> >;

class TileMap : public Manager{
    ///< We create this as a Manager child class because it acts as one
    ///< (managing multiple entities, which are the tile blocks).
    private:
        std::string name; ///< A name for the tile map (may help debugging):

        // Game window related:
        int x0, y0;                         ///< Where to start drawing the tile map
        int tile_width, tile_height;        ///< Dimensions of the tiles in the game window

        // Texture related:
        std::string texture_id;             ///< Texture ID of the whole 
        int sprite_width, sprite_height;    ///< Dimensions of the tiles in the .bmp file
        int nb_ids, ids_per_row;            ///< Number of textures (total) and how many per row.

        // map grid information:
        int nb_xtiles, nb_ytiles;           ///< Nb of tiles in each dimension (in the screen)
        map_shape map;                      ///< Map grid information.
        std::vector< std::array<int, 2> > texture_pos;   ///< ``texture_pos[id][0] = x, texture_pos[id][1] = y``

    public:
        // Call order:
        /** Constructor. This will only set up some basic members.
        @param mName: A name to give the TileMap object (mostly for debugging).
        @param mTwidth: the tile width in game (will become a destination rectangle's width).
        @param mTheight: analogous to mTwidth but for the height.
        */
        TileMap(std::string mName, int mTwidth, int mTheight): 
            name(mName), x0(0), y0(0), tile_width(mTwidth), tile_height(mTheight){};

        void init(std::string texture_id, std::string metadata_path);
        void setup(int x_tiles, int y_tiles);
        void load_map(std::string path);    ///< Load the tile IDs and collision information from a map text file.
        void set_position(int x, int y);    ///< This can be used to change where the grid starts.
        
        // TODO: add possibility to update other map variables and optimize rendering. 
};