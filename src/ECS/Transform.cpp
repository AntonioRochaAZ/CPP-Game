#include "Transform.hpp"

void Transform::update(){
    // Real velocity vector: speed * velocity.
    if (velocity.norm() != 0.0){
        position += speed * (velocity/velocity.norm());
    }
}