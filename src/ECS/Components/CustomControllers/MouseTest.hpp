#pragma once
#include "../MouseController.hpp"

struct MouseTest : public MouseController{
    bool m_clicked;
    vec m_shift;
    MouseTest(): m_clicked(false), m_shift(vec(0,0)) {};
    void update() override;
    void on_left_click() override;
    void on_left_click_end() override;
};