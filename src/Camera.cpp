#include "Game.hpp"
#include "ECS/Components.hpp"
    

// The following window information is defined in Game.cpp
extern Manager manager;

/**
 *   Update scaling and shifting information when window size changes.
 */
void Camera::window_update(){
    int w_width, w_height;
    int game_width, game_height;
    SDL_GetWindowSize(Game::m_window, &w_width, &w_height);
    #ifdef DEBUG_MODE
        Mix_PlayChannel(-1, Game::assets.get_audio("death"), 0);
        std::cout << "New window size: " << w_width << " " << w_height << std::endl;
    #endif
    if (w_width >= WINDOW_ASPECT_RATIO * w_height){
        // In this case, the height is the limiting dimension.
        game_height = w_height;
        game_width = WINDOW_ASPECT_RATIO * game_height;
    } else{
        // In this case, the width is the limiting dimension
        game_width = w_width;
        game_height = ((float) w_width / (float) WINDOW_ASPECT_RATIO);
    }
    m_pos_shift[0] = ((float) (w_width - game_width))/2.0;
    m_pos_shift[1] = ((float) (w_height - game_height))/2.0;
    // In the next line, it doesn't matter whether we take height or width
    // because game_heigt and game_width, like MAX_HEIGHT and MAX_WIDTH
    // differ by the same factor (the aspect ratio).
    m_scale_factor = ((float) game_height)/ ((float) MAX_HEIGHT);
};

/** Get scaled and shifted SDL_Rect for rendering (render_rect), according to in-game postions calculated (dst_rect).
 * 
 */
SDL_Rect Camera::get_render_rect(SDL_Rect dst_rect){

    dst_rect.x -= m_pos.x(); // Shifting the position on-screen according to camera position.
    dst_rect.y -= m_pos.y(); // (basically going from in-game coordinates to camera coordinates)

    SDL_Rect render_rect;       // Now going to render coordinates:
    render_rect.x = std::round(m_pos_shift.x() + m_scale_factor * dst_rect.x);
    render_rect.y = std::round(m_pos_shift.y() + m_scale_factor * dst_rect.y);
    render_rect.w = std::round(dst_rect.w * m_scale_factor);
    render_rect.h = std::round(dst_rect.h * m_scale_factor);

    return render_rect;
};

/**
 * Updates camera position at each tick. Handles following player too.
 */
void Camera::update_pos(){
    // Updating camera position:
    if (m_tracking_player){ 
        if (m_ref_entity.expired()){ update_ref_entity(); }
        // If it isn't nullptr:
        if (!m_ref_entity.expired()){
            // Getting new position:
            vec new_ref_entity_position = m_ref_entity.lock()->getComponent<Transform>().position;
            // Updating camera position:
            m_pos = m_previous_pos + \
                (new_ref_entity_position - m_previous_ref_entity_pos);

            // Updating previous camera position:
            m_previous_pos = m_pos;
            // Updating ref_entity_position:k
            m_previous_ref_entity_pos = new_ref_entity_position;
        }
    }
}

/**
 * Updates the reference entity which the camera is following.
 * This depends on the \ref ::Manager object defined in Game.cpp, which
 * stores the players.
 */
void Camera::update_ref_entity(){
    // Try to get a new player as the reference entity:
    if (manager.grouped_entities[PLAYER_GROUP].size() != 0){
        // I think here this won't work: we would need to reach out for the
        // regular shared pointer in the manager's entity vector.
        Entity*& player_ptr =  manager.grouped_entities[PLAYER_GROUP][0];
        std::shared_ptr<Entity> found_ptr;
        for (std::shared_ptr<Entity> sh_ptr : manager.entity_vector){
            if (sh_ptr.get() == player_ptr){
                found_ptr = sh_ptr;
                break;
            }
        }
        m_ref_entity = found_ptr;

    }else{
        m_tracking_player = false;
    }
}

void Camera::toggle_follow_player(){
    if (m_tracking_player){
        // This condition must be met, otherwise
        // will have a bug where neither the map not
        // the player doesn't move
        m_tracking_player = false;
    }else{
        m_tracking_player = true;
        // Get current reference camera and entity positions (will be update next):
        m_previous_pos = m_pos;

        // Deal with camera_ref_entity if the reference entity has died:
        if (m_ref_entity.expired()){
            update_ref_entity();
        }
        // If it isn't anymore, update position.
        if (!m_ref_entity.expired()){ 
            m_previous_ref_entity_pos = m_ref_entity.lock()->getComponent<Transform>().position;
        }
    }
}
