#pragma once
#include "ECS.hpp"
#include <eigen3/Eigen/Dense>

using vec = Eigen::Vector2f;

class Transform : public Component{
    private:
        vec position;       // (pixel, pixel)
        float speed;
        vec velocity;       // (pixel/frame, pixel/frame), normalized
        //vec acceleration;   // (pixel/frame**2, pixel/frame**2)

    public:
        // Constructors: ------------------------------------------------------
        // Default (no arguments):
        Transform():
            position(vec(0.0, 0.0)),
            speed(3),
            velocity(vec(0.0, 0.0))
            //acceleration(vec(0.0, 0.0))
        {};
        // Setting position:
        Transform(float x, float y):
            speed(3), 
            velocity(vec(0.0, 0.0))
        {
            set_position(x, y);
        }

        // Base class methods: ------------------------------------------------
        // void init() override {};
        void update() override;     // Defined in Transform.cpp (conciseness)

        // Position related: --------------------------------------------------
        float get_x(){ return position[0]; }
        float get_y(){ return position[1]; }
        void set_x(float x){ position[0] = x; }
        void set_y(float y){ position[1] = y; }
        void set_position(float x, float y){ position = vec(x, y); }
        
        // Velocity related: --------------------------------------------------
        // Speed:
        float get_speed(){ return speed; }
        void set_speed(float s){ speed = s; }
        // Velocity vector:
        float get_vx(){ return velocity[0]; }
        float get_vy(){ return velocity[1]; }
        void set_vx(float x){ velocity[0] = x; }
        void set_vy(float y){ velocity[1] = y; }
        void set_velocity(float x, float y){ velocity = vec(x, y); }
        
        //void set_acceleration(float x, float y){
        //    velocity = vec(x, y);
        //}
};