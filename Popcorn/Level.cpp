#include "Level.h"

//------------------------------------------------------------------------------------------------------------------------
char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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
ALevel::ALevel()
: Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0), Level_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
    Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255));

    AsConfig::Create_Pen_And_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    AsConfig::Create_Pen_And_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    
    Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
    Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
    Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
    Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double& ball_direction)
{
    // корректируем движение шарика от кирпичей
    int brick_y_pos = AsConfig::Level_Y_Offset + AsConfig::Level_Height * AsConfig::Cell_Height;
    
    for(int i = AsConfig::Level_Height - 1; i >=0; i--)
    {
        for(int j = 0; j < AsConfig::Level_Width; j++)
        {
            if(Level_01[i][j] == 0)
                continue;

            if(next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_direction = -ball_direction;
            }
        }

        brick_y_pos -= AsConfig::Cell_Height;
    }
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{ // отривка 
    RECT intersection_rect;
    
    if(!IntersectRect(&intersection_rect, &paint_area, &Level_Rect)) return;
    
    for (int i = 0; i < AsConfig::Level_Height; i++)
        for (int j = 0; j < AsConfig::Level_Width; j++)
            Draw_Brick(hdc, (EBrick_Type)Level_01[i][j], AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height);

    Active_Brick.Draw(hdc, paint_area);
    
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, EBrick_Type brick_type, int x, int y)
{// отрисовка кирпича
    
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

    RoundRect(hdc,
        x * AsConfig::Global_Scale, y * AsConfig::Global_Scale,
        (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale,
        2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{// вывод падающей буквы
    if(is_switch_color)
    {
        front_pen = Brick_Blue_Pen;
        front_brush = Brick_Blue_Brush;

        back_pen = Brick_Red_Pen;
        back_brush = Brick_Red_Brush;
    }
    else
    {
        front_pen = Brick_Red_Pen;
        front_brush = Brick_Red_Brush;

        back_pen = Brick_Blue_Pen;
        back_brush = Brick_Blue_Brush;
    }
}
//------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{ // вывод падающей буквы
    bool switch_color;
    double offset;
    double rotation_angle;
    int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
    int back_part_offset;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xForm, oldXForm;

    if(!(brick_type == EBT_Blue || brick_type == EBT_Red)) return;

    // корректируем шаг вращения и угол поворота
    rotation_step = rotation_step % 16;
    
    if(rotation_step < 8)
        rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    else
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step);
    
    if(rotation_step > 4 && rotation_step <= 12)
        brick_type == EBT_Blue ? switch_color = true : switch_color = false;
    else
        brick_type == EBT_Red ? switch_color = true : switch_color = false;
    
    Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);
    
    if(rotation_step == 4 || rotation_step == 12)
    {// выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height);
        // выводим передений план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height + AsConfig::Global_Scale - 1);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        // настраиваем матрицу вращения буквы
        xForm.eM11 = 1.0f;
        xForm.eM12 = 0.0f;
        xForm.eM21 = 0.0f;
        xForm.eM22 = (float)cos(rotation_angle);
        xForm.eDx  = (float)x;
        xForm.eDy  = (float)y + float(brick_half_height);
        GetWorldTransform(hdc, &oldXForm);
        SetWorldTransform(hdc, &xForm);

        // выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);
        
        offset = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)AsConfig::Global_Scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height - back_part_offset);

        // выводим передений план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);
    
        Rectangle(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

        if(rotation_step > 4 && rotation_step <=12)
        {
            if(letter_type == ELT_O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, (-5 * AsConfig::Global_Scale) / 2, 0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
            }
        }

        SetWorldTransform(hdc, &oldXForm);
    }
}
//------------------------------------------------------------------------------------------------------------------------