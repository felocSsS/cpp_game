﻿#pragma once

#include "Config.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
    ABall();

    void Init();
    
    void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);
    void Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width);

private:
    double Ball_Direction, Ball_Speed;
    int Ball_X_Pos, Ball_Y_Pos;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;
    RECT Ball_Rect, Prev_Ball_Rect;
};
//------------------------------------------------------------------------------------------------------------------------
