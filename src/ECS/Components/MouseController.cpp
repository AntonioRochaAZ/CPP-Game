#include "MouseController.hpp"
// #include "../../utils.hpp"


void MouseController::handle_events(SDL_Event *event){
    // on_hover(); // Not anymore
    if (event->type == SDL_MOUSEBUTTONDOWN){
        switch (event->button.button){
            case SDL_BUTTON_LEFT:
                on_left_click();
                m_hover_update[HoverActionBind::UPDATE_LEFT_DOWN] = true;
                break;
            case SDL_BUTTON_RIGHT:
                on_right_click();
                m_hover_update[HoverActionBind::UPDATE_RIGHT_DOWN] = true;
                break;
            default:
                break;
        }
    } else if (event->type == SDL_MOUSEBUTTONUP){
        switch (event->button.button){
            case SDL_BUTTON_LEFT:
                on_left_click_end();
                m_hover_update[HoverActionBind::UPDATE_LEFT_UP] = true;
                break;
            case SDL_BUTTON_RIGHT:
                on_right_click_end();
                m_hover_update[HoverActionBind::UPDATE_RIGHT_UP] = true;
                break;
            default:
                break;
        }
    }
}

void MouseController::handle_hover_click(){
    on_hover();
    
    if (m_hover_update[HoverActionBind::UPDATE_LEFT_DOWN]){
        on_hover_left_click();
    } else if (m_hover_update[HoverActionBind::UPDATE_LEFT_UP]){
        on_hover_left_click_end();
    }

    if (m_hover_update[HoverActionBind::UPDATE_RIGHT_DOWN]){
        on_hover_right_click();
    } else if (m_hover_update[HoverActionBind::UPDATE_RIGHT_UP]){
        on_hover_right_click_end();
    }
}

// void MouseController::on_hover(){ // Test function
//     entity->addComponent<Spawning>(300);
// }
