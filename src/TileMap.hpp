#pragma once
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Game.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include <stdexcept>    // Exception throwing

using map_shape = std::vector<std::vector<int>>;

class TileMap : public Manager{
    // We create this as a Manager child class because it acts as one.
    private:
        // A name for the tile map (may help debugging):
        std::string name;

        // Texture related:
        SDL_Texture* texture;           // The texture pointer
        int image_width, image_height;  // W and H of the actual texture.
        int sprite_width, sprite_height;    // Dimensions of the tiles in the .bmp file
        int nb_ids, ids_per_row;    // Number of textures (total) and how many per row.

        // Game window related:
        int x0, y0;                         // Where to start drawing the tile map
        int tile_width, tile_height;        // Dimensions of the tiles in the game window
        int nb_xtiles, nb_ytiles;           // Nb of tiles in each dimension (in the screen)

        // map grid information:
        map_shape map;
        std::vector< std::array<int, 2> > texture_pos;   // [id][0] = x, [id][1] = y

    public:

        // So we can keep track of the player's movement (update map rendering).
        Entity* tile_player;

        // Call order:
        TileMap(std::string mName): name(mName), x0(0), y0(0){};
        void init(int sw, int sh, int ni, int ipr, std::tuple< SDL_Texture*, int, int> texture_tuple);
        void set_dst_size(int tw, int th);
        void setup(int x_tiles, int y_tiles);
        void load_map(std::string path);

        // Setting up some variables:
        void set_position(int x, int y);

        void update_tracking();

        // TODO: add possibility to update other map variables.
};