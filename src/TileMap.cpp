#include "TileMap.hpp"

#include <cmath>
#include <fstream> // Reading files
#include <sstream> // ostringstream definition
#include <memory>
// So we can use the switch statement with strings:
#include <unordered_map>
#include "utils.hpp"

using map_shape = std::vector< std::vector<int> >;

// Call order: ------------------------------------------------------------

/** Initialize TileMap with a given texture file and its metadata.
This will touch texture related members (texture_id, sprite width and height, 
nb_ids, ids_per_row and texture_pos).
    
@param texture_id: the ID under which data is stored in the AssetManager.
@param metadata_path: the path to the text file in which information about
    the tilemap textures is stored (namely the shape of each texture, and the
    total number of IDs). The number of IDs per row is calculated automatically
    according to the size of the image. Note that this is not the map file.
*/
void TileMap::init(std::string mTexture_id, std::string metadata_path){
    // Variable declarations: -------------------------------------------------------------------------------
    std::ifstream metadata_file;
    std::string line;
    std::string token;
    int image_width;
    //-------------------------------------------------------------------------------------------------------
    // Defining some members:
    texture_id = mTexture_id;

    // Get information from meta data:
    metadata_file.open(metadata_path);
    if (!metadata_file.is_open()){
        std::cout << "ERROR could not open TileMap metadata file:";
        throw std::invalid_argument(metadata_path);
    }
    line = check_get_line(metadata_file, metadata_path); // Only one line to read
    // Variable declarations: -------------------------------------------------------------------------------
    std::stringstream ss(line);                // A variable we can tokenize
    //-------------------------------------------------------------------------------------------------------
    for (int i = 0; i < 3; i++){                // Get data
        if (!ss.good()){throw std::runtime_error("Error in TileMap::init: Trying to read more values than there are tokens.");}
        std::getline(ss, token, ',');   // Getting value
        switch(i){
        case 0:
            std::istringstream(token) >> nb_ids;
            break;
        case 1:
            std::istringstream(token) >> sprite_width;
            break;
        case 2:
            std::istringstream(token) >> sprite_height;
            break;
        }
    }

    // Calculating the number of IDs per row, for calculating
    // where each ID starts:
    check_map_id<std::string, int>(Game::assets.width_map, texture_id, "TileMap::init, Game::assets.width_map");
    image_width = Game::assets.width_map[texture_id];
    ids_per_row = image_width / sprite_width;

    // Setting texture_pos (ID positions):
    for (int id = 0; id < nb_ids; id++){
        texture_pos.emplace_back(
            std::array<int, 2>{
                (id % ids_per_row) * sprite_width,
                (id / ids_per_row) * sprite_height
            }
        );
    }
}

/** Sets up the Entity vectors, according to the number of tiles passed. It will set up
information about the position and sprite of the tiles, although their IDs will only be
set later (in \ref load_map), by effectively changing where the source rectangle is read 
from in the base texture file. 

Ideally, in the future, this should be set up automatically in \ref load_map.

@param x_tiles: Number of tiles (x axis).
@param y_tiles: Number of tiles (y axis).
*/
void TileMap::setup(int xtiles, int ytiles){
    // Sets up the entity_vector

    nb_xtiles = xtiles; nb_ytiles = ytiles;
    std::ostringstream full_name;   // String we can concatenate variables (composing the name)
    std::string final_name;         // The final string.

    // If we already had entries in the entity_vector, let's destroy them and refresh. 
    // May not be optimal, but it is much simpler. We won't often do this anyway.
    for (std::unique_ptr<Entity>& e : entity_vector){ e->destroy(); }
    refresh();

    // Adding Tile entities to the entity_vector
    for (int y = 0; y < nb_ytiles; y ++){
        // std::vector< std::unique_ptr<Tile> > vec;
        std::vector<int> map_line;
        for (int x = 0; x < nb_xtiles; x++){
            // Getting the tile name for debugging:
            full_name.str("");      // Resetting to be empty
            full_name << "TileMap: " << name << " - (" << x << "," << y << ")";
            final_name = full_name.str();
            Entity& e = this->addEntity(final_name);
            e.add_group(MapGroup);
            e.addComponent<Transform>(
                x0 + x * tile_width, 
                y0 + y * tile_height);
            e.addComponent<Sprite>(texture_id);

            e.getComponent<Transform>().set_speed(0.0);
            e.getComponent<Sprite>().set_src_height(sprite_height);
            e.getComponent<Sprite>().set_src_width(sprite_width);
            e.getComponent<Sprite>().set_dst_height(tile_height);
            e.getComponent<Sprite>().set_dst_width(tile_width);
            map_line.emplace_back(-1);
        }
        map.emplace_back(map_line);
    }
}


void TileMap::load_map(std::string path){
    std::ifstream map_file;         // File type
    std::string line;              // Variable used for reading lines.

    std::unordered_map<std::string, int> cases = {
        {"tex", 1},
        {"col", 2},
    };

    map_file.open(path);
    if (!map_file.is_open()){
        std::cout << "ERROR: Could not open map file:";
        throw std::invalid_argument(path);
    }

    // Let's first get the number of x and y tiles:
    line = check_get_line(map_file, path);
    std::stringstream ss(line);     // A variable we can tokenize
    std::string token;              // The numbers, as a string
    int xtiles, ytiles;             // int where we are going to store the entry.               
    for (int i = 0; i < 2; i++){
        if (!ss.good()){
            throw std::runtime_error("Error in TileMap::load_map: trying to read more tokens than there tokens/line in the text file: nb of x and y tiles.");
        }
        std::getline(ss, token, ',');       // Getting the number as a str
        // Turning it into an int:
        switch(i){
        case 0:
            std::istringstream(token) >> xtiles;
            break;
        case 1: 
            std::istringstream(token) >> ytiles;
            break;
        }
    }
    setup(xtiles, ytiles);  // Setting the entities up.

    // Loop for reading through file's lines:
    while (std::getline(map_file, line)) {
    switch (cases[line.substr(0, 3)]) {
    case 1: // Texture information
        for (int y = 0; y < nb_ytiles; y++){
                // Read line and check for errors:
                line = check_get_line(map_file, path);

                std::stringstream ss(line);      // A variable we can tokenize
                std::string token;               // The numbers, as a string
                
                for (int x = 0; x < nb_xtiles; x++){

                    if (!ss.good()){
                        throw std::runtime_error(
                            "Error in TileMap::load_map: trying to read more tokens than there tokens/line in the text file.");
                    }

                    // Getting the number as a str:
                    std::getline(ss, token, ',');
                    // Turning it into an int:
                    int id;
                    std::istringstream(token) >> id;

                    // Update tile source position according to id:
                    Sprite& s = entity_vector[nb_xtiles*y + x]->getComponent<Sprite>();
                    s.set_src_x(texture_pos[id][0]);
                    s.set_src_y(texture_pos[id][1]);
                    map[x][y] = id;
                }
                // map_file.get(tile);     // A number.
                // // atoi turns a string into an int.
                // // std::cout << "Tile " << y*10 + x << ": " << tile;
                // Game::add_tile(x0 + x*tile_width, y0 + y*tile_height, tile_width, tile_height, atoi(&tile), final_name);
                // map_file.ignore();      // A comma.
            // }
        } // closing case tex
        break;
    case 2: // collision information.
        for (int y = 0; y < nb_ytiles; y++){
                // Read line and check for errors:
                line = check_get_line(map_file, path);

                std::stringstream ss(line);      // A variable we can tokenize
                std::string token;               // The numbers, as a string

                for (int x = 0; x < nb_xtiles; x++){

                    if (!ss.good()){
                        throw std::runtime_error(
                            "Error in TileMap::load_map: trying to read more tokens than there tokens/line in the text file.");
                    }

                    // Getting the number as a str:
                    std::getline(ss, token, ',');
                    // Turning it into an int:
                    int id;
                    std::istringstream(token) >> id;

                    // add collision if id is 1:
                    if (id == 1){
                        std::string local_name = entity_vector[nb_xtiles*y + x]->get_name();
                        entity_vector[nb_xtiles*y + x]->addComponent<Collider>(
                            local_name, tile_width, tile_height);
                    }
                }
                // map_file.get(tile);     // A number.
                // // atoi turns a string into an int.
                // // std::cout << "Tile " << y*10 + x << ": " << tile;
                // Game::add_tile(x0 + x*tile_width, y0 + y*tile_height, tile_width, tile_height, atoi(&tile), final_name);
                // map_file.ignore();      // A comma.
            // }
        } // closing case col
    default: // Can't interpret it:
        continue; // Do nothing, just cycle.
    } // closing switch
    } // closing while loop.
    map_file.close();
}

void TileMap::set_position(int x, int y){
    for(std::unique_ptr<Entity>& e : entity_vector){
        e->getComponent<Transform>().set_x(e->getComponent<Transform>().get_x() + (x - x0));
        e->getComponent<Transform>().set_y(e->getComponent<Transform>().get_y() + (y - y0));
    }
    x0 = x;  y0 = y; 
}
