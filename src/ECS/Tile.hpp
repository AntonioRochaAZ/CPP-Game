#pragma once
#include "ECS.hpp"
#include "Transform.hpp"
#include "Sprite.hpp"
#include "SDL2/SDL.h"

class Tile : public Component{
    public:
        Transform* transform;
        Sprite* sprite;

        SDL_Rect tile_rect;
        int tile_id;
        char* path;

        Tile() = default;
        Tile(float x, float y, int w, int h, int id){
            tile_rect.x = x;
            tile_rect.y = y;
            tile_rect.w = w;
            tile_rect.h = h;
            tile_id = id;

            switch (tile_id){
                case 0:
                    path = "assets/stone0.bmp";
                    break;
                case 1:
                    path = "assets/stone1.bmp";
                    break;
                default:
                    break;
            }
        }

        void init(){
            entity->addComponent<Transform>(
                tile_rect.x, tile_rect.y, tile_rect.w, tile_rect.h);
            transform = &entity->getComponent<Transform>();
            
            entity->addComponent<Sprite>(path);
            sprite = &entity->getComponent<Sprite>();
            sprite->init();
        }
};