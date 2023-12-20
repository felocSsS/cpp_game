﻿#pragma once

#include "Config.h"

enum EPlatform_State
{
    EPS_Missing,
    EPS_Normal,
    EPS_Meltdown
};

//------------------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
    AsPlatform();

    void Init();
    void Act(HWND hwnd);
    void Redraw(HWND hwnd);
    void Draw(HDC hdc, RECT& paint_area);

    int X_Pos;
    int Width;
    int X_Step;

private:
    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
    EPlatform_State Platform_State;
    int Inner_Width;
    static const int Normal_Width = 28;
    int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

    HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen ;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
    RECT Platform_Rect, Prev_Platform_Rect;
    
    static const int Circle_Size = 7;
    static const int Height = 7;
    static const int Meltdown_Speed = 7;
};
//------------------------------------------------------------------------------------------------------------------------