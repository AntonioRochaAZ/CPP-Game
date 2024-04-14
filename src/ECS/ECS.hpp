#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <cmath>

enum group_labels : std::size_t{
    MapGroup,
    ColliderGroup,
    EnemyGroup,
    ProjectileGroup,
    PlayerGroup,
};

// Forward declarations (so they can be recognized when
// referrenced) before the actual definitions:
class Component;
class Entity;
class Manager;

// In the next line: "std::size_t is the unsigned integer type of
// the result of the sizeof operator as well as the sizeof...
// operator and the alignof operator(since C++11)."
// (https://en.cppreference.com/w/cpp/types/size_t)
using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID(){
    /*
    The "inline" keyword here is an optimization. Since this
    function is small and will be called often, using "inline"
    will actually write down the function instructions as if
    they were written directly in the code where the function
    is called. This optimizes code at runtime.
    
    The fact that we define lastID as a static variable
    makes it so that each time we call the function, the
    previous value it had is kept. So, it is initialized at
    zero and, since we have lastID++, it increments by one
    each time the function is called. It will become clear why
    this is useful in the next definition in this source code
    (template <typename T> inline ComponentID
    getComponentTypeID() noexcept).
    */
    static ComponentID lastID = 0u; // The u makes it unsigned.
    lastID++;
    return lastID;
}

template <typename T> 
inline ComponentID getComponentTypeID() noexcept{
    /*
    The template "allows the compiler to write code for you"
    (https://www.youtube.com/watch?v=I-hZkUa9mIs&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=54&ab_channel=TheCherno)
    The template creates a blueprint.
    
    Here, the template would replace "T" for the type
    specified when calling the function. We can see that T
    isn't used anywhere.
    Basically, when a new Component is created, we'll generate
    an unique ID for it by calling this function and passing
    the component class as T. As such:
    ID = getComponentTypeID<ComponentClass>().
    C++ understands that this function is different from the
    previous because we pass the template type T, had we not
    specified it, C++ would understand we're calling the
    previous function.
    
    By defining the static variable typeID here, typeID will
    be remembered everytime this function is called for the
    same T type. It is initialized as getComponentTypeID()
    (the previous function, in this case). This means that,
    everytime we instantiate a NEW type T and call
    getComponentTypeID<T>(), we'll set T's typeID as the
    lastID defined in getComponentTypeID(). Because typeID is
    not changed inside this template, everytime we call
    getComponentTypeID<T>(), we'll get the same typeID (it is
    only initialized as lastID the first time!). This means
    that each component is associated to exactly one typeID,
    and that each component class has a unique ID because
    everytime getComponentTypeID() is called it generates a
    new ID. 
    
    This seems like an overly complicated way of doing this,
    but it most definitely guarantees that we'll not mess up
    the ID of component classes by hardcoding them.
    */
    static_assert (std::is_base_of<Component, T>::value, "");
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

// constexpr means that the value of the variable can be
// evaluated at compile time.
//
// Max amount of components an entity is capable of holding.
constexpr ComponentID max_components = 32;
// Maximum number of groups/layers. Each entity can be member of
// multiple layers. Groups can be used for render layers as well
// as collision layers later on.
constexpr Group max_groups = 32;

/*
"The class template bitset represents a fixed-size sequence of
N bits. Bitsets can be manipulated by standard logic operators
and converted to and from strings and integers. For the purpose
of the string representation and of naming directions for shift
operations, the sequence is thought of as having its lowest
indexed elements at the right, as in the binary representation of
integers."
Source: https://en.cppreference.com/w/cpp/utility/bitset

"Bitset class emulates space efficient array of boolean values,
where each element occupies only one bit."
Source:
https://www.tutorialspoint.com/cpp_standard_library/bitset.htm

According to the tutorial, this will serve for comparing if an
entity has component X in its components. Probably more related
to this last detail.

The template for the bitset class takes in a variable type
std::size_t (here renamed as ComponentID), in practice a value.
This value defines the "size" of the bitset "vector".
*/
using ComponentBitSet = std::bitset<max_components>;
// Bitset for checking if an entity is part of a group.
using GroupBitSet = std::bitset<max_groups>;

/* Definition of std::array:
template <typename T, std::size_t N> struct array;

"The struct combines the performance and accessibility of a
C-style array with the benefits of a standard container, such as
knowing its own size, supporting assignment, random access
iterators, etc."
https://en.cppreference.com/w/cpp/container/array

"It has friendly value semantics, so that it can be passed to or
returned from functions by value. Its interface makes it more
convenient to find the size, and use with STL-style
iterator-based algorithms."
https://stackoverflow.com/questions/30263303/what-are-the-advantages-of-using-stdarray-over-c-style-arrays

Array of Component pointers with the length of our
max_components.
*/
using ComponentArray = std::array<Component*, max_components>;

// Defining the generic component class, which will be used
// subsequently for other specific ones (see Components.hpp):
class Component{
    public:
        // Each component instance will have an entity associated
        // to it.
        Entity* entity;

        /*
        "A virtual function (also known as virtual methods) is a
        member function that is declared within a base class and
        is re-defined (overridden) by a derived class. When you
        refer to a derived class object using a pointer or a
        reference to the base class, you can call a virtual
        function for that object and execute the derived class’s
        version of the method."
        Source: https://www.geeksforgeeks.org/virtual-function-cpp/
        */
        virtual void init(){};
        virtual void update(){};
        virtual void render(){};
        virtual ~Component(){};
};

class Entity{
    public:
    //protected:
        // Reference to the manager the entity is attached to.
        Manager& manager;
        std::string name;   // A name to identify the entity
        // The "active" bool will be used for checking if we
        // should remove the entity from the game (if it is false
        // it will be removed).
        bool active = true;
        /*
        "std::unique_ptr is a smart pointer that owns and manages
        another object through a pointer and disposes of that
        object when the unique_ptr goes out of scope."
        Source: https://en.cppreference.com/w/cpp/memory/unique_ptr
        */
        std::vector< std::unique_ptr<Component> > components;
        
        // Array of components that the entity has:
        ComponentArray component_array;
        // Bitset array containing information of which
        // component types the entity has:
        ComponentBitSet component_bitset;
        // Bitset array containing information of which
        // groups the entity is part of:
        GroupBitSet group_bitset;

    //public:
        // Constructor with a manager passed. The "manager"
        // member is initialized as the user_manager passed.
        Entity(Manager& user_manager) :
            manager(user_manager){}
        Entity(Manager& user_manager, std::string mName) :
            manager(user_manager)
            {name = mName;}
        virtual ~Entity(){};
        
        std::string get_name(){return name;}
        void set_name(std::string mName){name = mName;}
        virtual void update(){
            for (auto& c: components) c->update();
        }
        virtual void render(){
            for (auto& c: components) c->render();
            // This is where we'll add rendering related to the entity.
        }
        bool is_active() const { return active; }
        void destroy(){ active = false; }
        

        template <typename T> bool has_component() const{
            // When we query this function with object type T, it
            // will return whether the entity has it or not.
            return component_bitset[getComponentTypeID<T>()];
        }
        bool has_group(Group group){
            return group_bitset[group];
        }
        /*
        In the next lines we have "...", when looking at this
        website: "https://en.cppreference.com/w/cpp/language/template_parameters"
        at "Non-type template parameter" number 3, we see it
        relates to "parameter packs". It links us to this link:
        https://en.cppreference.com/w/cpp/language/parameter_pack,
        which says: "A template parameter pack is a template
        parameter that accepts zero or more template arguments
        (non-types, types, or templates). A function parameter
        pack is a function parameter that accepts zero or more
        function arguments."

        This means that the first template argument, T, is
        mandatory and is a class, but that the template may
        receive extra, optional arguments. Here, these arguments
        are called Targs, and because of the preceeding
        "typename" keyword, they must be classes. It seems to me
        that "... Targs" serves a similar purpose to python's
        "*args" in a function input.

        So we can pass something like addComponent<T, TArg1,
        TArg2>.
        
        T will define the class of the method's return value, and
        it will be a reference (because of the ampersand); while
        Targs will define the types of the functions actual
        inputs mArgs. A call would look like:

        addComponent<T, Targ1, Targ2>(Targ1&& mArg1, Targ2&& mArg2);
        
        For example, say we want to add Component "POSITION" to the
        entity, and that to initialize it we'll need arguments
        x, y of types int. We'll call:
        entity.addComponent<POSITION, int, int>(x, y).
        We can see it since in the very first line we define a T*
        pointer called c, taking mArgs as input to create the T instance.
        

        */
        // "the idea is to do something like:
        // entity.getComponent<Transform>().setXpos()"
        // (said in the tutorial).
        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs){
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
        }
        void add_group(Group group);
        void del_group(Group group){
            group_bitset[group] = false;
        }

        /*
        This next template returns the entity's component
        as a reference when calling the template with the
        respective component class. For example:
        entity.getComponent<POSITION> returns the entity's
        transform component class as a reference
        */
        template<typename T>
        T& getComponent() const{
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
        }
};

// TODO: in the addEntity, should delete the "new" entities at
// some point... Instead, create smart pointers directly, and
// then return them instead of regular pointers.
class Manager{
    /*
    This class serves to manage all entities present in the game
    at each frame. It will delete entities that are not active.
    */
    protected:
        // Vector of entities:
        std::vector< std::unique_ptr<Entity> > entity_vector;
        // An array containing the vector of entities of each
        // group. We can think of it as a python list with 32
        // entries, each one being a list of entity pointers.
        std::array< std::vector<Entity*>, max_groups > grouped_entities;

    public:
        void update(){
            /* 
            Here, auto will get type std::unique_ptr<Entity>.
            We use auto for generality and readability. We use
            the ampersand to get a reference because, had we not
            done it, "e" would be copies of the entities in the
            entity vector and not the actual objects we want to
            update. That is, without the ampersand, we would be
            updating copies of the objects that we want to
            update, and not the objects themselves.

            This code was understood with the help of ChatGPT 3.5.
            */
            for (auto& e: entity_vector) e->update();
            this->refresh();
        }
        void render(){
            // for (auto& e: entity_vector) e->render();
            for (auto& e_vec : grouped_entities){
                // Go through the entity groups in order
                for (auto& e : e_vec){
                    // Render the entities
                    e->render();
                }
            }
        }
        void refresh(){
            
            // Analogous to the next line statement (which is
            // more detailed).
            // Here, i is initialized with direct initialization.
            // I don't know why he's specified unsigned integer.
            for (auto i(0u); i < max_groups; i++){
                // We initialize variable v as entry i of the
                // group_entities member. Because we use auto,
                // the type will be understood directly from the
                // initialized value. Note it is a referrence.
                auto& v(grouped_entities[i]);
                // We do as below: we remove entries of the
                // vector (so, Entity pointers) that aren't
                // active OR that are not part of the group.
                // TODO: DELETE THOSE POINTERS! (?)
                v.erase(
                    std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* mEntity){
                        return !mEntity->is_active() || !mEntity->has_group(i);
                    }),
                    std::end(v)
                );
            }
            
            /*
            The following lines have been modified from the
            tutorial for readability.

            According to ChatGPT 3.5: 
                "-std::remove_if is an algorithm that takes a range of
                elements and a predicate function. 
                -It rearranges the elements in the range so that the
                elements satisfying the predicate are moved to the
                end, and the rest are moved to the front. 
                -It returns an iterator pointing to the beginning of
                the "removed" elements (i.e., the elements that
                should be erased)."

                "Yes, std::remove_if operates in-place on the
                container. It doesn't create a copy of the container;
                instead, it rearranges the elements within the
                container itself."
                 
            [](const std::unique_ptr<Entity> &mEntity) is a
            lambda function. The "[]" is the lambda introducer;
            like python's "lambda".
            */
            auto beginning = std::remove_if(
                    std::begin(entity_vector), std::end(entity_vector),
                    [](const std::unique_ptr<Entity> &mEntity){
                        return !mEntity->is_active();
                    }
                );
            // Erase entries of entity_vector starting at
            // transform beginning and ending at transform
            // std::end(entity_vector) (the end of the vector).
            entity_vector.erase(beginning,  std::end(entity_vector)
            );
        }

        void add_to_group(Entity* entity, Group group){
            // THIS FUNCTION IS CALLED BY Entity's "add_group"
            // method, and shouldn't be called directly
            // (otherwise it will not update Entity's
            // group_bitset member).
            // We add entity to the group
            grouped_entities[group].emplace_back(entity);
        }

        // Function for getting the vector of entities in a
        // group.
        std::vector<Entity*>& get_group(Group group){
            return grouped_entities[group];
        }

        Entity& addEntity(std::string mName = "None"){
            Entity* e = new Entity(*this, mName);
            return addEntity(e);
        }
        // Overloading:
        Entity& addEntity(Entity* e){
            std::unique_ptr<Entity> uPtr{e};
            entity_vector.emplace_back(std::move(uPtr));
            return *e;
        }
        // virtual ~Manager(){};
};