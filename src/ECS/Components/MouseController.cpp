#include "MouseController.hpp"
// #include "../../utils.hpp"


void MouseController::handle_events(){
    on_hover();
    if (Game::event.type == SDL_MOUSEBUTTONDOWN){
        switch (Game::event.button.button){
            case SDL_BUTTON_LEFT:
                on_left_click();
                break;
            case SDL_BUTTON_RIGHT:
                on_right_click();
                break;
            default:
                break;
        }
    } else if (Game::event.type == SDL_MOUSEBUTTONUP){
        switch (Game::event.button.button){
            case SDL_BUTTON_LEFT:
                on_left_click_end();
                break;
            case SDL_BUTTON_RIGHT:
                on_right_click_end();
                break;
            default:
                break;
        }
    }
}

// void MouseController::on_hover(){ // Test function
//     entity->addComponent<Spawning>(300);
// }
