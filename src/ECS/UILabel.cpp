#include "UILabel.hpp"

void UILabel::set_text(std::string mText, std::string mFont_id){
    // TODO: Change this function so that we can change all text options here.
    // Otherwise, we could've just called it from the constructor.

    // TODO: is there any point in defining these members if they are
    // only used to set the text once?
    text = mText; font_id = mFont_id;

    // TODO: do we need to delete this pointer later?
    SDL_Surface* surface = TTF_RenderText_Blended(
        Game::assets->get_font(font_id), text.c_str(), colour);
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);   // Does this delete the pointer?

    SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
}

void UILabel::render(){
    SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
}