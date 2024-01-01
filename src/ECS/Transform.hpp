#pragma once
#include "ECS.hpp"
#include <eigen3/Eigen/Dense>

using vec = Eigen::Vector2f;

class Transform : public Component{
    public:

        float speed;
        vec position;       // (pixel, pixel)
        vec velocity;       // (pixel/frame, pixel/frame), normalized
        //vec acceleration;   // (pixel/frame**2, pixel/frame**2)
        //int x_scale, y_scale;
        int width, height;

        Transform():
            speed(3),
            position(vec(0.0, 0.0)),
            velocity(vec(0.0, 0.0)),
            //acceleration(vec(0.0, 0.0))
            //x_scale(1),
            //y_scale(1),
            width(0),
            height(0)
        {};

        Transform(float x, float y, int w, int h){
            position[0] = x;
            position[1] = y;
            width = w;
            height = h;
        }

        float x(){ return position[0]; }
        void x(float x){ position[0] = x; }
        float y(){ return position[1]; }
        void y(float y){ position[1] = y; }
        float w(){ return width; }
        float h(){ return height; }

        void init() override {
            //speed = 1;
            //position = vec(0.0, 0.0);
            //velocity = vec(0.0, 0.0);
            //velocity = vec(0.75, 0.24); // pixels/frame
            //acceleration = vec(0.0, 0.0);
            //if (entity->has_component<Sprite>()){
            //    width = entity->getComponent<Sprite>().get_width();
            //    height = entity->getComponent<Sprite>().get_height();
            //}
        }
        void set_position(float x, float y){
            position = vec(x, y);
        }
        void set_velocity(float x, float y){
            velocity = vec(x, y);
        }
        //void set_acceleration(float x, float y){
        //    velocity = vec(x, y);
        //}
        void set_width(int w){
            width = w;
        }
        void set_height(int h){
            height = h;
        }

        void update() override {
            // Real velocity vector: speed * velocity.
            if (velocity.norm() != 0.0){
                position += speed * (velocity/velocity.norm());
            }
        }
};