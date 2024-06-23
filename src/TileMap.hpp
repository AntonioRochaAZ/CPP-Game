#pragma once
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Game.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include <stdexcept>    // Exception throwing

using map_shape = std::vector<std::vector<int>>;

class TileMap : public Manager{
    ///< We create this as a Manager child class because it acts as one
    ///< (managing multiple entities, which are the tile blocks).
    private:
        std::string name; ///< A name for the tile map (may help debugging):

        // Texture related:
        std::string texture_id;             ///< Texture ID of the whole 
        int sprite_width, sprite_height;    ///< Dimensions of the tiles in the .bmp file
        int nb_ids, ids_per_row;            ///< Number of textures (total) and how many per row.

        // Game window related:
        int x0, y0;                         ///< Where to start drawing the tile map
        int tile_width, tile_height;        ///< Dimensions of the tiles in the game window
        int nb_xtiles, nb_ytiles;           ///< Nb of tiles in each dimension (in the screen)

        // map grid information:
        map_shape map;      ///< Map grid information.
        std::vector< std::array<int, 2> > texture_pos;   ///< ``texture_pos[id][0] = x, texture_pos[id][1] = y``

    public:
        // Call order:
        TileMap(std::string mName): name(mName), x0(0), y0(0){};

        /** Initialize the TileMap.
        @param texture_id: the ID under which data is stored in the AssetManager.
        @param metadata_path: the path to the text file in which information about
            the tilemap textures is stored (namely the shape of each texture, and the
            total number of IDs). The number of IDs per row is calculated automatically
            according to the size of the image.
        @param mTwidth: the tile width in game (will become a destination rectangle's width).
        @param mTheight: analogous to mTwidth but for the height.
        */
        void init(std::string texture_id, std::string metadata_path, int mTwidth, int mTheight);
        // void init(int sw, int sh, int ni, int ipr, std::string texture_id);

        /** Sets up the Entity vectors, according to the number of tiles passed. It will set up
        information about the position and sprite of the tiles, although their IDs will only be
        set later (in \ref load_map), by effectively changing where the source rectangle is read 
        from in the base texture file.
        @param x_tiles: Number of tiles (x axis).
        @param y_tiles: Number of tiles (y axis).
        */
        void setup(int x_tiles, int y_tiles);

        /// Load the ID and other data from a text file.
        void load_map(std::string path);

        void set_position(int x, int y);    ///< This can be used to change where the grid starts.

        // TODO: add possibility to update other map variables and optimize rendering. 
};