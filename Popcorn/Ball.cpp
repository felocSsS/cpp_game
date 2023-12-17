#include "Ball.h"

//------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
    : Ball_Direction(M_PI - M_PI_4), Ball_Pen(0), Ball_Brush(0), Ball_X_Pos(20), Ball_Y_Pos(170),
      Ball_Speed(3.0), Ball_Rect{}, Prev_Ball_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
    AsConfig::Create_Pen_And_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
    RECT intersection_rect;
    if(!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect)) return;
    
    // очищаем фон
    SelectObject(hdc, bg_pen);
    SelectObject(hdc, bg_brush);

    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1); 

    // рисуем шарик
    SelectObject(hdc, Ball_Pen);
    SelectObject(hdc, Ball_Brush);

    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);  
}
//------------------------------------------------------------------------------------------------------------------------
void ABall::Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width)
{
    int next_x_pos, next_y_pos;
    int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
    int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;
    
    Prev_Ball_Rect = Ball_Rect;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

    // корректируем движение шарика от рамки 
    if(next_x_pos < AsConfig::Border_X_Offset)
    {
        next_x_pos = AsConfig::Level_X_Offset - (next_x_pos - AsConfig::Level_X_Offset);
        Ball_Direction = M_PI - Ball_Direction;
    }

    if(next_y_pos < AsConfig::Border_Y_Offset)
    {
        next_y_pos = AsConfig::Border_Y_Offset - (next_y_pos - AsConfig::Border_Y_Offset);
        Ball_Direction = -Ball_Direction;
    }

    if(next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }

    if(next_y_pos > AsConfig::Max_Y_Pos)
    {
        next_y_pos = AsConfig::Max_Y_Pos - (next_y_pos - AsConfig::Max_Y_Pos);
        Ball_Direction = M_PI + (M_PI - Ball_Direction);
    }

    // корректируем движение шарика от платформы
    if(next_y_pos > platform_y_pos)
    {
        if(next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
        {
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
            Ball_Direction = M_PI + (M_PI - Ball_Direction);
        }
    }

    level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

    // перемещаем шарик
    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;
    
    Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
    Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
    Ball_Rect.right = Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Global_Scale;
    Ball_Rect.bottom = Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Global_Scale;

    InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------------------