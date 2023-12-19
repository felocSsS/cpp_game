#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
    AsPlatform();

    void Init();
    void Redraw(HWND hwnd);
    void Draw(HDC hdc, RECT& paint_area);

    int X_Pos;
    int Width;
    int X_Step;

private:
    int Inner_Width;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen ;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
    RECT Platform_Rect, Prev_Platform_Rect;
    
    static const int Circle_Size = 7;
    static const int Height = 7;
};
//------------------------------------------------------------------------------------------------------------------------