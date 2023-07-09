#include "Engine.h"

enum EBrick_Type
{
    EBT_None,
    EBT_Red,
    EBT_Blue
};

HPEN Brick_Red_Pen;
HBRUSH Brick_Red_Brush;
HPEN Brick_Blue_Pen;
HBRUSH Brick_Blue_Brush;

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;

char Level_01[14][12] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//------------------------------------------------------------------------------------------------------------------------
void InitPensAndBrushes()
{
    Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
    Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

    Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
    Brick_Blue_Brush = CreateSolidBrush(RGB(85, 255, 255));
}
//------------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, EBrick_Type brick_type, int x, int y)
{
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Red:
        pen = Brick_Red_Pen;
        brush = Brick_Red_Brush;
        break;

    case EBT_Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;

    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//------------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 12; j++)
            Draw_Brick(hdc, (EBrick_Type)Level_01[i][j], Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height);
}
//------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{
    Draw_Level(hdc);
}
//------------------------------------------------------------------------------------------------------------------------