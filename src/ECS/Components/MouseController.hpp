#pragma once
#include "../../Game.hpp"
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../States.hpp"

enum HoverActionBind : std::size_t{
    UPDATE_LEFT_DOWN = 0,
    UPDATE_RIGHT_DOWN= 1,
    UPDATE_LEFT_UP = 2,
    UPDATE_RIGHT_UP = 3 
};

/* Component for allowing controlling entities through the mouse. */
struct MouseController : public Component{

    MouseController(){};

    std::array<bool, 4> m_hover_update;
    Transform* transform;
    Sprite* sprite;
    Collider* collider;

    void init() override { render(); };

    void render() override {
        // Small cheat to flush update array.
        for(int i = 0; i < 4; i++){ 
            m_hover_update[i] = false; 
        }
    };

    /** Function for getting the Transform and Sprite components. This is separate from the init
    function in case the MouseController is defined before those other components and we have
    to get them later.*/
    void get_components(){
        if (entity->has_component<Transform>()){ transform = &entity->getComponent<Transform>(); }
        if (entity->has_component<Sprite>()){ sprite = &entity->getComponent<Sprite>(); }
        // The entity must have a collider so we can check if the click was in the entity:
        collider = &entity->getComponent<Collider>();
    };

    // The next methods are for when we click anywhere:
    void handle_events(SDL_Event *event);
    virtual void on_left_click(){};
    virtual void on_right_click(){};
    virtual void on_left_click_end(){};
    virtual void on_right_click_end(){};

    // The next methods are for when we click while hovering over the player (called in collision.cpp, though shouldn't really work anymore...):
    void handle_hover_click();
    virtual void on_hover(){};
    virtual void on_hover_left_click(){};
    virtual void on_hover_right_click(){};
    virtual void on_hover_left_click_end(){};
    virtual void on_hover_right_click_end(){};
};
