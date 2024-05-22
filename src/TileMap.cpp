#include "TileMap.hpp"

#include <cmath>
#include <fstream> // Reading files
#include <sstream> // ostringstream definition
#include <memory>
// So we can use the switch statement with strings:
#include <unordered_map>


using map_shape = std::vector< std::vector<int> >;

// Call order: ------------------------------------------------------------
void TileMap::init(int sw, int sh, int ni, int ipr,
    std::tuple<SDL_Texture*, int, int> texture_tuple){

    // Load texture file and set some variables up.
    std::tie(texture, image_width, image_height) = texture_tuple;
    sprite_width = sw; sprite_height = sh;
    nb_ids = ni; ids_per_row = ipr;

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
            full_name << "TileMap: " << name << " - (" << x << "," << y << ")";
            final_name = full_name.str();
            Entity& e = this->addEntity(final_name);
            e.add_group(MapGroup);
            e.addComponent<Transform>(
                x0 + x * tile_width, 
                y0 + y * tile_height);
            std::tuple<SDL_Texture*, int, int> texture_tuple = {texture, image_width, image_height};
            e.addComponent<Sprite>(texture_tuple, false);

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

    auto get_line = [&]() -> std::string {
        /* Checks if we've reached the end of a file we're working on before
        we should. This function will throw runtime errors if so. The path is
        only used to aid debugging.

        I tried to make this general and save it in a utils.hpp file,
        but it did not work because of the ifstream variable:
        "error: call to implicitly-deleted copy constructor of 'std::ifstream'
        (aka 'basic_ifstream<char>'"
        I'll try to fix this eventually.
        */
            // Checking for errors:
            if (map_file.eof()){
                std::cout << "ERROR WHEN READING FILE: " << path;
                throw std::runtime_error(
                    "Error when reading file: EOS reached.");
            }
            // Reading the line into variable "line":
            if (!std::getline(map_file, line)){
                std::cout << "ERROR WHEN READING FILE: " << path;
                throw std::runtime_error(
                    "Error when reading line.");
        }
        return line;
    };

    map_file.open(path);
    if (!map_file.is_open()){
        std::cout << "ERROR: Could not open map file:";
        throw std::invalid_argument(path);
    }
    // Loop for reading through file's lines:
    while (std::getline(map_file, line)) {
    switch (cases[line.substr(0, 3)]) {
    case 1: // Texture information
        for (int y = 0; y < nb_ytiles; y++){
                // Read line and check for errors:
                line = get_line();

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
                line = get_line();

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

void TileMap::set_position(int x, int y){ x0 = x;  y0 = y; }
