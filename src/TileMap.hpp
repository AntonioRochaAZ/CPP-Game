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
        
        template<std::size_t xtiles, std::size_t ytiles>
        void load_map(bool redo_setup, int id_array[xtiles][ytiles], std::string type);    
            ///< Load the tile IDs or collision information from an array of ints. We use double pointer
            ///< to avoid having to declare the shape before-hand.
        void set_position(int x, int y);    ///< This can be used to change where the grid starts.
        
        // TODO: add possibility to update other map variables and optimize rendering. 
};

/// Eventually we could couple this with the previous function, so that this function is called by the other.
template<std::size_t xtiles, std::size_t ytiles>
void TileMap::load_map(bool redo_setup, int id_array[xtiles][ytiles], std::string type){

    std::unordered_map<std::string, int> cases = {
        {"tex", 1},
        {"col", 2},
    };

    if (redo_setup){ setup(xtiles, ytiles); }   // Setting the entities up. 
    
    switch (cases[type.substr(0, 3)]) {
    case 1: // Texture information
        for (int y = 0; y < nb_ytiles; y++){
                // Read line and check for errors:
                for (int x = 0; x < nb_xtiles; x++){
                    int id = id_array[x][y];
                    // Update tile source position according to id:
                    Sprite& s = entity_vector[nb_xtiles*y + x]->getComponent<Sprite>();
                    s.src_rect.x = texture_pos[id][0];
                    s.src_rect.y = texture_pos[id][1];
                    map[y][x] = id; // line number, then column
                }
        } // closing case tex
        break;
    case 2: // collision information.
        for (int y = 0; y < nb_ytiles; y++){
                for (int x = 0; x < nb_xtiles; x++){
                    int id = id_array[x][y];
                    // add collision if id is 1:
                    if (id == 1){
                        std::string local_name = entity_vector[nb_xtiles*y + x]->get_name();
                        entity_vector[nb_xtiles*y + x]->addComponent<Collider>(
                            local_name, CollisionHandle::IMMOVABLE, tile_width, tile_height);
                    }
                }
        } // closing case col
    default: // Can't interpret it:
        break;
        // continue; // Do nothing, just cycle.
    } // closing switch
};
