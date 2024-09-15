#include "MouseTest.hpp"

void MouseTest::on_left_click(){
    m_clicked = true;
    // Next line is to avoid the player to move with relation to the cursor
    // when it is clicked.
    m_shift = entity->getComponent<Transform>().position - Game::cursor->getComponent<Transform>().position;
}

void MouseTest::on_left_click_end(){
    m_clicked = false;
    m_shift = vec(0,0);
}

void MouseTest::update(){
    if (m_clicked){
        entity->getComponent<Transform>().position = m_shift + Game::cursor->getComponent<Transform>().position;
    }
}