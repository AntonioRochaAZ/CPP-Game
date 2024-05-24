#include "UILabel.hpp"

void UILabel::set_text(std::string mText, std::string mFont_id){
    // TODO: Change this function so that we can change all text options here.
    // Otherwise, we could've just called it from the constructor.

    // TODO: is there any point in defining these members if they are
    // only used to set the text once?
    text = mText; font_id = mFont_id;

    // TODO: do we need to delete this pointer later?
    TTF_SetFontSize(Game::assets.get_font(font_id), font_size);
    SDL_Surface* surface = TTF_RenderUTF8_Solid(
        Game::assets.get_font(font_id), text.c_str(), colour);
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);   // Does this delete the pointer?

    SDL_QueryTexture(texture, nullptr, nullptr, &dst_rect.w, &dst_rect.h);
}

void UILabel::update(){
    dst_rect.x = static_cast<int>(transform->get_x() + shift[0]);
    dst_rect.y = static_cast<int>(transform->get_y() + shift[1]);
}

void UILabel::render(){
    dst_rect.x -= Game::camera_position[0]; // Updating position according to camera.
    dst_rect.y -= Game::camera_position[1];
    SDL_RenderCopy(Game::renderer, texture, nullptr, &dst_rect);
}