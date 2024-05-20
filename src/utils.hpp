#pragma once
#include <string>

/** Function for checking if the queried entry of a map exists (``map[id]``), 
    and throw an error otherwise.

    This is used to avoid undefined behaviour in the code and ease
    debugging.

    @tparam T1: The type of the ID.
    @tparam T2: The type of the map entry.

    @param map: The map we want to verify.
    @param id: The ID we want to verify.
    @param message: A message containing the function that's behind
        this call and the name of the map involved, in that function's
        scope. This will be concatenated to the printed output if an
        error is thrown, like so:
        ``std::cout << "Function and map names: " << message << "; ID: " << id << std::endl;``
*/
template <typename T1, typename T2>
bool check_map_id(std::map<T1, T2> map, T1 id, std::string message, bool halt_execution = true){

    typename std::map< T1, T2 >::iterator map_iter = map.find(id);

    if (map_iter == map.end()){
        if (halt_execution){
            std::cout << "ERROR: ID passed to Map is unknown.\n";
            std::cout << "Function and map names: " << message << "; ID: " << id << std::endl; 
            throw std::invalid_argument("utils.hpp: check_map_id");
        }else{
            return false;
        }
    }else{
        return true;
    }
};