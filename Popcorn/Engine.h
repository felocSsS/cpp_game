#pragma once

#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

enum EKey_Type
{
    EKT_Left,
    EKT_Right,
    EKT_Space
};

enum EBrick_Type
{
    EBT_None,
    EBT_Red,
    EBT_Blue
};

enum ELetter_Type
{
    ELT_None,
    ELT_O
};

const int Timer_ID = WM_USER + 1;

class AsEngine
{
public:
    AsEngine();
    void Init_Engine(HWND hwnd);
    void Draw_Frame(HDC hdc, RECT& paint_area);
    int On_Key_Down(EKey_Type Key_Type);
    int On_Timer();

    static const int Level_Width = 12; // in cells
    static const int Level_Height = 14;

private:
    void Create_Pen_And_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
    void Redraw_Platform();
    void Draw_Brick(HDC hdc, EBrick_Type brick_type, int x, int y);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
    void Draw_Level(HDC hdc);
    void Draw_Platform(HDC hdc, int x, int y);
    void Draw_Ball(HDC hdc, RECT& paint_area);
    void Draw_Border(HDC hdc, int x, int y, bool top_border);
    void Draw_Bounds(HDC hdc, RECT& paint_area);
    void Check_Level_brick_Hit(int &next_y_pos);
    void Move_Ball();
    
    HWND Hwnd;
    HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen, BG_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, BG_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;
    RECT Platform_Rect, Prev_Platform_Rect;
    RECT Level_Rect;
    RECT Ball_Rect, Prev_Ball_Rect;
    int Inner_Width;
    int Platform_X_Pos;
    int Platform_X_Step;
    int Platform_Width;
    int Ball_X_Pos, Ball_Y_Pos;
    double Ball_Speed, Ball_Direction;

    static const int Global_Scale = 4;
    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Level_X_Offset = 8;// in cells
    static const int Level_Y_Offset = 6;
    static const int Circle_Size = 7;
    static const int Platform_Y_Pos = 185;
    static const int Platform_Height = 7;
    static const int Ball_Size = 4;
    static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
    static const int Max_Y_Pos = 199 - Ball_Size;
    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;
};

