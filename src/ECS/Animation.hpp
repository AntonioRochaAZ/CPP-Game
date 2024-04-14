#pragma once

struct Animation{
    int frames;             // Number of frames in the animation
    int animation_period;   // Period of each frame in miliseconds
    int sprite_width;
    int sprite_height;
    
    // Members set up by the Sprite component:
    int index;
    int src_height;

    Animation(){};
    Animation(int f, int t, int w, int h):
        frames(f), animation_period(t), sprite_width(w), sprite_height(h),
        index(-1), src_height(0){
    }

};