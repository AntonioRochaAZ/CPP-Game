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
        SDL_Texture* texture;           ///< The texture pointer
        int image_width, image_height;  ///< W and H of the actual texture.
        int sprite_width, sprite_height;    ///< Dimensions of the tiles in the .bmp file
        int nb_ids, ids_per_row;    ///< Number of textures (total) and how many per row.

        // Game window related:
        int x0, y0;                         ///< Where to start drawing the tile map
        int tile_width, tile_height;        ///< Dimensions of the tiles in the game window
        int nb_xtiles, nb_ytiles;           ///< Nb of tiles in each dimension (in the screen)

        // map grid information:
        map_shape map;      ///< Map grid information.
        std::vector< std::array<int, 2> > texture_pos;   ///< ``texture_pos[id][0] = x, texture_pos[id][1] = y``

    public:
        Entity* tile_player; ///< So we can keep track of the player's movement (update map rendering).

        // Call order:
        TileMap(std::string mName): name(mName), x0(0), y0(0){};

        /** Initialize the TileMap.
        @param sw: Width of a block texture in the source image in pixels (all blocks must have the same shape).
        @param sh: Height of a block texture in the source image in pixels (all blocks must have the same shape).
        @param ni: The number of different textures (IDs) present in the texture file.
        @param ipr: The number of IDs per row (in the texture file).
        @param texture_tuple: The tuple returned by \ref Game::AssetManager::get_tuple. 
            This will contain the texture loaded from the texture file, which contains the
            texture for all block IDs. When being rendered, we'll simply render a different
            part of this texture.
        */
        void init(int sw, int sh, int ni, int ipr, std::tuple< SDL_Texture*, int, int> texture_tuple);

        /** Defines the size of the tiles (in the actual game window).
        @param tw: Tile width (pixels).
        @param th: Tile height (pixels).
        */
        void set_dst_size(int tw, int th);

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

        /** This next function changes if the grid is stationary or if it moves (this will)
        depend on whether the camera is following the player or not. This is only called in 
        \ref Game::handle_events as of now, and may change in the future, depending on how
        we deal with the POV. */
        void update_tracking(); 

        // TODO: add possibility to update other map variables and optimize rendering. 
};