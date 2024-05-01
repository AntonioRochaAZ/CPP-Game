#pragma once
#include "Transform.hpp"
#include "../../Game.hpp"  // ECS will be included here


class UILabel : public Component{
private:
    vec shift;              ///< How much to shift the text box from the Transform's position.
    std::string text;       ///< The actual text to display.
    std::string font_id;    
        ///< The ID of the font to be used (will be loaded through \ref Game::assets::get_font).
    SDL_Color colour;       ///< The color of the object.

    SDL_Rect dst_rect;      ///< The destination rectangle to be rendered.
    SDL_Texture* texture;   
        ///< The texture object to be rendered (managed by the component, 
        ///< and destroyed by the destructor).
    Transform* transform;   
        ///< The Entity's \ref ::Transform component for the position.
        ///< Here we use a pointer, so that we don't have to explicitly 
        ///< initialize it in the constructor.

public:

    /** Constructor
        @param x_shift: How much to shift the text box in the x-axis, in pixels
                        (with respect to the Entity's Transform::position).
        @param y_shift: Analogously, in the y-axis (pixels).
        @param width: The width of the destination rectangle (pixels).
        @param height: The height if the destination rectangle (pixels).
        @param mText: The text to be displayed.
        @param mFont_id: The font_id (see member).
        @param mColour: The text color.
    */
    UILabel(float x_shift, float y_shift, int width, int height, std::string mText, std::string mFont_id, SDL_Color& mColour):
        shift(vec(x_shift, y_shift)), text(mText), font_id(mFont_id), colour(mColour){
        dst_rect.w = width;
        dst_rect.h = height;
        set_text(mText, mFont_id);
    };
    /// The destructor (destroys the texture pointer).
    ~UILabel(){ SDL_DestroyTexture(texture); };

    /** Initialization function: it sets the transform member and initializes the
        destination rectangle position according to the shift. It appears that trying 
        to set the transform member in the constructor doesn't work (probably 
        something related to not having access to the entity member or something like
        that). */
    void init() override{
        transform = &entity->getComponent<Transform>();  // Get the transform for position. 
        dst_rect.x = static_cast<int>(transform->get_x() + shift[0]);
        dst_rect.y = static_cast<int>(transform->get_y() + shift[1]);
    }

    void set_text(std::string mText, std::string mFont_id); 
        ///< Sets the text to be displayed. Called in the constructor.
    void update() override; 
        ///< Updates dst_rect's position according to the transform's (defined in UILabel.cpp).
    void render() override;

    /** Function for hooking up this component to another Entity's transform. This can be
        useful if we want to have multiple text boxes following a same Entity. */
    void set_transform(Transform* t){ transform = t; }
};