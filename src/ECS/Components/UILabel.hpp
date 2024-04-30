#pragma once
#include "../../Game.hpp"  // ECS will be included here

class UILabel : public Component{
public:
    UILabel(int xpos, int ypos, int width, int height, std::string mText, std::string mFont_id, SDL_Color& mColour):
        text(mText), font_id(mFont_id), colour(mColour){
        position.x = xpos;
        position.y = ypos;
        position.w = width;
        position.h = height;
        set_text(mText, mFont_id);
    };
    ~UILabel(){};

    void set_text(std::string mText, std::string mFont_id);
    void render() override;

private:
    SDL_Rect position;
    std::string text;
    std::string font_id;
    SDL_Color colour;
    SDL_Texture* texture;
};