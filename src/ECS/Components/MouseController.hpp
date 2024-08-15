#pragma once
#include "../../Game.hpp"
#include "../ECS.hpp"
#include "../Components.hpp"
#include "../States.hpp"

using vec = Eigen::Vector2f;

/* Component for allowing controlling entities through the mouse. */
struct MouseController : public Component{

    MouseController(){};

    Transform* transform;
    Sprite* sprite;
    Collider* collider;

    void init() override {};

    /** Function for getting the Transform and Sprite components. This is separate from the init
    function in case the MouseController is defined before those other components and we have
    to get them later.*/
    void get_components(){
        if (entity->has_component<Transform>()){ transform = &entity->getComponent<Transform>(); }
        if (entity->has_component<Sprite>()){ sprite = &entity->getComponent<Sprite>(); }
        // The entity must have a collider so we can check if the click was in the entity:
        collider = &entity->getComponent<Collider>();
    };

    void handle_events();
    virtual void on_hover(){};
    virtual void on_left_click(){};
    virtual void on_right_click(){};
};
