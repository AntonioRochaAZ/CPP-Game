#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <cmath>

#include "SDL.h"

// Type definitions: ==============================================================================
using ComponentID = std::size_t;    // TODO: rename to ComponentIdType
using Group = std::size_t;          // TODO: rename to GroupType

constexpr ComponentID max_components = 32;  ///< Max amount of components an entity is capable of holding.
constexpr Group max_groups = 32;            ///< Max. number of groups (layers).

using ComponentBitSet = std::bitset<max_components>;    ///< Used for checking if an Entity has a given Component.
using GroupBitSet = std::bitset<max_groups>;            ///< Used for checking if an Entity is part of a group.

/// Enumerator for groups of entities (cannot be an enum class because its values are used in std::size_t
/// dependant functions):
enum EntityGroup : std::size_t{
    NO_GROUP,
    MAP_GROUP,
    COLLIDER_GROUP,
    TEMPORARY_GROUP,
    PLAYER_GROUP,
    ATTACK_GROUP,
};

// Forward declarations (so they can be recognized when referrenced) before the actual definitions:
class Component;
class Entity;
class Manager;

/// Array of Component pointers.
using ComponentArray = std::array<Component*, max_components>;

// Component ID Handling: =========================================================================
/** Function for generating a new ID when a new Component is defined.
This function is only called in getComponentTypeID, if the Component hasn't yet been assigned an ID.
This is done by using a static variable, which is incremented everytime this function is called. */
inline ComponentID getNewComponentTypeID(){
    static ComponentID lastID = 0u; // The u makes it unsigned.
    lastID++;
    return lastID;
}

/** Returns the ID of the passed Component. This is done by using a template, specifying
the Component type, and a static variable so that the function remembers the ID from one call to 
the next. */
template <typename T> 
inline ComponentID getComponentTypeID() noexcept{
    static_assert (std::is_base_of<Component, T>::value, "");   // I think only checks if type T is a Component.
    static ComponentID typeID = getNewComponentTypeID();        // Get's the unique ID
    return typeID;
}

// ECS definition: ================================================================================
/** Definition of the generic component class, which will be subsequently used for other specific 
ones (see Components.hpp). */
class Component{
protected:
    bool active = true;

public:
    // TODO : use a shared pointer? Or not even necessary? Maybe
    // a reference? What is the interest of using a reference
    // again?
    Entity* entity; ///< Pointer to the Entity it is associated to.

    virtual void init(){}; 
        ///< Component initialization. This is called in Entity::addComponent and should have no
        ///< arguments.
    virtual void update(){};    ///< Update function (called every game tick). 
    virtual void render(){};    ///< Rendering (called every game tick).
    virtual ~Component(){};     

    bool is_active(){ return active; }
    void destroy(){ active = false; }
};

/** Definition of the generic Entity class. */
class Entity{
protected:
    bool active = true; 

public:
    Manager& manager; ///< Reference to the manager the entity it is attached to.
    std::string name; ///< A name to identify the entity
        ///< Used to decide whether to delete an entity or not at each game tick.
        ///< This is handled in Manager::refresh.  
    std::vector< std::unique_ptr<Component> > components;
        ///< Vector of the Entity's Components (as unique pointers).
        ///< Entries are emplaced in the order they are added to the entity.
    ComponentArray component_array;
        ///< Array of components that the entity has. Here, entry k corresponds 
        ///< to the Component with type ID k (see \ref getComponentTypeID function).
    ComponentBitSet component_bitset;
        ///< Bitset array containing information of which Component types the entity has.
    GroupBitSet group_bitset;
        ///< Bitset array containing information of which groups the entity is part of.

    Entity(Manager& user_manager, std::string mName);
    virtual ~Entity(){};

    void refresh();

    /// Default update function: updates each component.
    virtual void update(){ 
        int end = components.size();    // Avoid size changes during loop
        for (int i = 0; i < end; i++) { components[i]->update(); }
        refresh();
    }
    /// Default render function: renders each component.
    virtual void render(){ 
        int end = components.size();    // Avoid size changes during loop
        for (int i = 0; i < end; i++) { components[i]->render(); }
    }

    /// Returns whether the Entity has a given Component. @tparam T: A Component type.
    template <typename T> bool has_component() const{
        return component_bitset[getComponentTypeID<T>()]; }
    
    /// Returns whether the Entity is part of a given group.
    bool has_group(Group group){ return group_bitset[group]; }

    /** Function for adding a component T to the Entity (defined in the end of ECS.hpp).
        @tparam T: A Component type. 
        @tparam Targs: The types of parameters mArgs (this must be defined in the 
            template so we can have a generic definition in the function).
        @param mArgs: T's constructor arguments. 
    */
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs);

    /** Function for getting one of the Entity's components (defined in the end of ECS.hpp).
        @tparam T: A Component type. 
    */
    template<typename T>
    T& getComponent() const;

    /** Function for adding the Entity to a group. This will call the Entity's manager's 
        Manager::add_to_group function. Because of this, this function is defined in ECS.cpp
        (because it requires full definition of the Manager class, and declaring outside 
        Entity's definition cause a linker "duplicate symbols" error). 
    */
    void add_group(std::size_t group);
    void del_group(std::size_t group){ group_bitset[group] = false; }


    // Set/Get functions:
    std::string get_name(){return name;}
    void set_name(std::string mName){name = mName;}
    bool is_active() const { return active; }
    /// Destroys the Entity by setting "active" member to false.
    /// The Entity's Manager will then delete it.
    virtual void destroy(){ active = false; }
};

/** This class serves to manage groups of entities in the game
at each frame. It will delete entities that are not active. */
class Manager{
    public:
    // protected:
        std::vector< std::shared_ptr<Entity> > entity_vector; 
            ///< Vector of entities. We define it as a shared pointer because we
            ///< want Game::camera_ref_entity to be a weak pointer of a given entity,
            ///< which requires it to be a shared_ptr. We don't want to share its 
            ///< ownership, though.
        std::array< std::vector<Entity*>, max_groups > grouped_entities; 
            ///< Array of vectors for each group.
    
        // ENTITY ADDITION:--------------------------------------------------------------
        /// Function for adding an entity to the \ref Manager::entity_vector "entity_vector"
        /// by creating a new pointer.
        std::shared_ptr<Entity>& addEntity(std::string mName = "None"){
            Entity* e = new Entity(*this, mName);   // We'll call addEntity(e) which will deal with the new pointer.
            return addEntity(e); 
        }
        /// Instead, add an existing entity to the \ref Manager::entity_vector "entity_vector".
        std::shared_ptr<Entity>& addEntity(Entity* e){
            std::shared_ptr<Entity> sh_ptr(e);        // Now the pointer is dealt with by unique_ptr
            return addEntity(sh_ptr);
        }
        std::shared_ptr<Entity>& addEntity(const std::shared_ptr<Entity> sh_ptr){
            entity_vector.emplace_back(sh_ptr);
            return entity_vector.back();
        }

        // GROUP HANDLING:---------------------------------------------------------------
        /// Add an entity to a group. This is called by Entity's Entity::add_group.
        /// It shouldn't be called directly. Defined in ECS.cpp.
        void add_to_group(Entity* entity, std::size_t group);
        /// Function for getting the vector of entities in a group.
        std::vector<Entity*>& get_group(Group group){ return grouped_entities[group]; };

        // MAIN LOOP FUNCTIONS:----------------------------------------------------------
        /// Function for handling keyboard and mouse click events.
        void handle_events(SDL_Event *event);
        /// Function for updating every entity in the Manager::entity_vector.
        void update(){
            int end = entity_vector.size(); // Necessary to avoid problems with entity_vector size changes during loop.
                // This will avoid problems with size increases, but not decreases...
            for (int i = 0; i < end; i++){ 
                entity_vector[i]->update(); // Update each entity.
            }
            this->refresh();                // Delete inactive ones.
        }
        /// Function for rendering every entity in the \ref Manager::entity_vector "entity_vector".
        /// This is done group by group, in the order defined in the group_labels enum.
        void render(){
            for (std::vector<Entity*> e_vec : grouped_entities){       // Go through the entity groups in order.
                int end = e_vec.size();
                    // Avoiding problems with e_vec size changes during loop (likely unnecessary here,
                    // but just in case). We don't need to do this for the outer loop since it is an
                    // array, having thus a fixed size.
                for (int i = 0; i < end; i++){
                    e_vec[i]->render(); // Render each entity.
                }   
            }
        }
        /// Checks for inactive Entity's and delete them. Also removes entities from groups that
        /// they don't belong. Defined in ECS.cpp.
        void refresh();
        
        virtual ~Manager(){};
};

// LONG DEFINITIONS: ==============================================================================
/* Here we include definitions that must be present in the header file, but were deemed too long to
be included in the class definitions. */

template <typename T, typename... TArgs>
T& Entity::addComponent(TArgs&&... mArgs){

    if(this->has_component<T>()){
        #ifdef DEBUG_MODE
            std::cout << "Trying to add Component that the entity already has: " << typeid(T).name() << std::endl;
        #endif
        return getComponent<T>();
    }
    // In the next line, T* c(...) is equivalent to 
    // T* c = ..., though these initializations may not
    // always yield the same results, according to the
    // case. We call these "Direct-Initializations" in
    // case you want to search more.
    T* c( new T(std::forward<TArgs>(mArgs)...) );           
    
    // "this" = python's "self", so we are defining the
    // component's entity attribute as this entity.
    c->entity = this;
    
    // We define a unique_ptr object of generic Component
    // type (not type T) called uPtr and we initialize it
    // as c.
    std::unique_ptr<Component> uPtr{ c };
    
    // Remember: this->components is an
    // std::vector<std::unique_ptr<Component>>
    // by calling std::vector's emplace_back method, we
    // insert "a new element at the end of the vector,
    // right after its current last element".
    // (https://cplusplus.com/reference/vector/vector/emplace_back/).
    // 
    // The "std::move" function is complex, here's more
    // information about it:
    // https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used
    components.emplace_back(std::move(uPtr));

    // Defining the ComponentTypeID's entry of component
    // array as the component instance
    component_array[getComponentTypeID<T>()] = c;
    // Defining the bitset's ComponentTypeID's entry as
    // true to say that we have included it
    component_bitset[getComponentTypeID<T>()] = true;

    // Initialize the component:
    c->init();

    // Return the value of the component as a reference.
    return *c;
};

template <typename T>
T& Entity::getComponent() const{
    #ifdef DEBUG_MODE
        if (!component_bitset[getComponentTypeID<T>()]){
            std::cout << "Entity " << this->name << " doesn't have the passed component (number " << getComponentTypeID<T>() << ")." << std::endl; 
            throw std::runtime_error("Entity::getComponent()");
        }
    #endif
    /*
    Here, the keyword "auto" is used because the type of
    the variable (named "ptr") is "complex". It could be
    replaced by Component*, though (since that is the
    type of component_array's entries).
    */
    auto ptr(component_array[getComponentTypeID<T>()]);
    /*
    However, the entries of component_array will actually
    be derived types(child classes of Component).
    Specifically the passed type "T". That is why, in the
    next lines we'll use static_cast to tell the code that,
    in actuality, ptr is actually of class T, using the
    static_cast to T*. We then have "*" before it so it
    returns the actual object (as a reference because the
    return value is a reference).

    This code was understood with the help of ChatGPT 3.5.
    */
    return *static_cast<T*>(ptr);
};