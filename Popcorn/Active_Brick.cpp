﻿#include "Active_Brick.h"

HPEN AActive_Brick::Fading_Blue_Brick_Pens[Max_Fade_Step];
HPEN AActive_Brick::Fading_Red_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Blue_Brick_Brushes[Max_Fade_Step];
HBRUSH AActive_Brick::Fading_Red_Brick_Brushes[Max_Fade_Step];

//------------------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type): Fade_Step(0), Brick_Type(brick_type)
{
}

//------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
    if(Fade_Step < Max_Fade_Step - 1)
    {
        ++Fade_Step;
        InvalidateRect(hwnd, &Brick_Rect, FALSE);
    }
}
//------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
    HPEN pen = 0;
    HBRUSH brush = 0;

    switch (Brick_Type)
    {
    case EBT_Blue:
        pen = Fading_Blue_Brick_Pens[Fade_Step];
        brush = Fading_Blue_Brick_Brushes[Fade_Step];
        break;
    case EBT_Red:
        pen = Fading_Red_Brick_Pens[Fade_Step];
        brush = Fading_Red_Brick_Brushes[Fade_Step];
        break;
    }
    
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
    Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
    Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
    
    RoundRect(hdc,
        Brick_Rect.left, Brick_Rect.top,
        Brick_Rect.right, Brick_Rect.bottom,
        2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Setup_Colors()
{
    for (int i = 0; i < Max_Fade_Step; i++)
    {
        Get_Fading_Color(AsConfig::Blue_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
        Get_Fading_Color(AsConfig::Red_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
    }
}
//------------------------------------------------------------------------------------------------------------------------
void AActive_Brick::Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush)
{
    unsigned char r,g,b;
    r = Get_Fading_Channel(AsConfig::Blue_Brick_Color.R, AsConfig::BG_Color.R, step);
    g = Get_Fading_Channel(AsConfig::Blue_Brick_Color.G, AsConfig::BG_Color.G, step);
    b = Get_Fading_Channel(AsConfig::Blue_Brick_Color.B, AsConfig::BG_Color.B, step);
    AsConfig::Create_Pen_And_Brush(r, g, b, pen, brush);
}
//------------------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
    return color - step * (color - bg_color) / Max_Fade_Step - 1;
}
//------------------------------------------------------------------------------------------------------------------------
