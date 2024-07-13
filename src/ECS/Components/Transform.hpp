#pragma once
#include "../ECS.hpp"
#include <eigen3/Eigen/Dense>

using vec = Eigen::Vector2f;

/** Component defining the position of an Entity on screen. Other components
such as the \ref ::Sprite and the \ref ::KeyboardController may depend on this 
Component.*/
struct Transform : public Component{
    // Members: ------------------------------------------------------
    vec position;    ///< Position on screen (pixel, pixel).
    float& x = position[0];     
    float& y = position[1];     
    float speed;                ///< Speed (pixels/tick).
    vec velocity;    
        ///< Velocity vector (pixel/tick, pixel/tick). It is normalized when
        ///< adjusting the position, and the speed member is used for the
        ///< magnitude.
    float& vx = velocity[0];       
    float& vy = velocity[1];  
    //vec acceleration;   ///< (pixel/tick**2, pixel/tick**2)

    // Constructors: ------------------------------------------------------
    /// Default constructor (no arguments). Initializes position and velocity to zero, but speed to 10.
    Transform(): position(vec(0.0, 0.0)), speed(10), velocity(vec(0.0, 0.0)){};
    /// Setting position (calls \ref Transform::set_position)
    Transform(float x, float y): speed(10), velocity(vec(0.0, 0.0)){ set_position(x, y); }
    /// Sets all variables up.
    Transform(vec pos, float spd, vec vel): position(pos), speed(spd), velocity(vel){}

    // Base class methods: ------------------------------------------------
    void update() override;     ///< Defined in Transform.cpp (conciseness)

    // Setter functions: --------------------------------------------------
    void set_position(float x, float y){ position = vec(x, y); }
    void set_velocity(float x, float y){ velocity = vec(x, y); }
};