#include "TileMap.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <fstream> // Reading files
#include <sstream> // ostringstream definition
#include <memory>

using map_shape = std::vector< std::vector<int> >;

// Call order: ------------------------------------------------------------
void TileMap::init(int sw, int sh, int ni, int ipr, std::string mPath){
    // Load texture file and set some variables up.
    sprite_width = sw; sprite_height = sh;
    nb_ids = ni; ids_per_row = ipr;
    texture_path = mPath;
    // Setting texture_pos:
    for (int id = 0; id < nb_ids; id++){
        texture_pos.emplace_back(
            std::array<int, 2>{
                (id % ids_per_row) * sprite_width,
                (id / ids_per_row) * sprite_height
            }
        );
    }
};
void TileMap::set_dst_size(int tw, int th){ tile_width = tw;  tile_height = th; }
void TileMap::setup(int xtiles, int ytiles){
    // Sets up the entity_vector

    nb_xtiles = xtiles; nb_ytiles = ytiles;
    std::ostringstream full_name;   // String we can concatenate variables (composing the name)
    std::string final_name;         // The final string.

    // Adding Tile entities to the entity_vector
    for (int y = 0; y < nb_ytiles; y ++){
        // std::vector< std::unique_ptr<Tile> > vec;
        std::vector<int> map_line;
        for (int x = 0; x < nb_xtiles; x++){
            // Getting the tile name for debugging:
            full_name.str("");      // Resetting to be empty
            full_name << "TileMap: " << name  << " - (" << x << "," << y << ")";
            final_name = full_name.str();
            Entity& e = this->addEntity(final_name);
            e.add_group(MapGroup);
            e.addComponent<Transform>(
                x0 + x * tile_width, 
                y0 + y * tile_height);
            e.addComponent<Sprite>(texture_path);
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
    map_file.open(path);
    if (!map_file.is_open()){
        std::cout << "ERROR: Could not open map file:";
        throw std::invalid_argument(path);
    }
        for (int y = 0; y < nb_ytiles; y++){
                // Read line:
                std::string line;      

                // Checking for errors:
                if (map_file.eof()){
                    throw std::runtime_error(
                        "Error in TileMap::load_map: trying to read more lines than there lines in the text file.");
                }

                // Reading the line into variable "line":
                if (!std::getline(map_file, line)){
                    throw std::runtime_error(
                        "Error in TileMap::load_map: error when reading line.");
                }

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
        }
    map_file.close();
}

void TileMap::set_position(int x, int y){ x0 = x;  y0 = y; }
