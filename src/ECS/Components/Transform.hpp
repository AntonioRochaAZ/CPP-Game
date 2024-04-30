#pragma once
#include "../ECS.hpp"
#include <eigen3/Eigen/Dense>

using vec = Eigen::Vector2f;

/** Component defining the position of an Entity on screen. Other components
such as the \ref ::Sprite and the \ref ::KeyboardController may depend on this 
Component.*/
class Transform : public Component{
    private:
        vec position;       ///< Position on screen (pixel, pixel).
        float speed;        ///< Speed (pixels/tick).
        vec velocity;       
            ///< Velocity vector (pixel/tick, pixel/tick). It is normalized when
            ///< adjusting the position, and the speed member is used for the
            ///< magnitude.
        //vec acceleration;   // (pixel/frame**2, pixel/frame**2)

    public:
        // Constructors: ------------------------------------------------------
        /// Default constructor (no arguments).
        Transform():
            position(vec(0.0, 0.0)),
            speed(10),
            velocity(vec(0.0, 0.0))
            //acceleration(vec(0.0, 0.0))
        {};
        /// Setting position (calls \ref Transform::set_position)
        Transform(float x, float y):
            speed(10), 
            velocity(vec(0.0, 0.0))
        {
            set_position(x, y);
        }
        /// Sets all variables up.
        Transform(vec pos, float spd, vec vel):
            position(pos),
            speed(spd),
            velocity(vel){}

        // Base class methods: ------------------------------------------------
        void update() override;     ///< Defined in Transform.cpp (conciseness)

        // Position related: --------------------------------------------------
        float get_x(){ return position[0]; }
        float get_y(){ return position[1]; }
        void set_x(float x){ position[0] = x; }
        void set_y(float y){ position[1] = y; }
        void set_position(float x, float y){ position = vec(x, y); }
        void set_position(vec pos){ position = pos; }   ///< Overloading.
        vec get_position(){return position;}
        // Velocity related: --------------------------------------------------
        // Speed:
        float get_speed(){ return speed; }
        void set_speed(float s){ speed = s; }
        // Velocity vector:
        float get_vx(){ return velocity[0]; }
        float get_vy(){ return velocity[1]; }
        vec get_velocity(){ return velocity; }
        void set_vx(float x){ velocity[0] = x; }
        void set_vy(float y){ velocity[1] = y; }
        void set_velocity(float x, float y){ velocity = vec(x, y); }
        void set_velocity(vec v){velocity = v;} ///< Overloading.
        
        //void set_acceleration(float x, float y){
        //    velocity = vec(x, y);
        //}
};