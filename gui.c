#include "gui.h"

#include <stddef.h>

bool gui_button(Rectangle bounds, Color color, Font font, float font_scale, const char *text)
{
    bool pressed = false;
    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, bounds) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            pressed = true;
        }
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
    }
    
    float font_size = bounds.width * bounds.width / GetScreenWidth() * font_scale;
    float FONT_SPACING = 1.0f;

    Vector2 text_size = MeasureTextEx(font, text, font_size, FONT_SPACING);
    Vector2 text_pos = {
        bounds.x + (bounds.width - text_size.x) / 2,
        bounds.y + (bounds.height - text_size.y) / 2
    };

    Rectangle back_bounds = bounds;
    bounds.x += 3;
    bounds.y += 3;
    bounds.width -= 6;
    bounds.height -= 6;

    Color back_color = color;
    back_color.r /= 3;
    back_color.g /= 3;
    back_color.b /= 4;

    DrawRectangleRounded(back_bounds, 0.3f, 5, back_color);
    DrawRectangleRounded(bounds, 0.3f, 5, color);
    DrawTextEx(font, text, text_pos, font_size, FONT_SPACING, WHITE);

    return pressed;
}

bool gui_slider(Rectangle bounds, Color color, float min, float max, float *value)
{
    bool changed = false;

    float control_width = bounds.width / 10;
    float control_height = bounds.height * 2;
    float control_x = bounds.x + (*value - min) / (max - min) * (bounds.width - control_width);
    Rectangle control = {
        .x = control_x,
        .y = bounds.y - control_height * 0.25f,
        .width = control_width,
        .height = control_height
    };

    if (CheckCollisionPointRec(GetMousePosition(), control) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        *value = min + (GetMouseX() - bounds.x - control_width / 2) / (bounds.width - control_width) * (max - min);
        if (*value < min)
            *value = min;
        if (*value > max)
            *value = max;
        changed = true;
    }

    Rectangle back_bounds = control;
    control.x += 3;
    control.y += 3;
    control.width -= 6;
    control.height -= 6;

    Color back_color = color;
    back_color.r /= 3;
    back_color.g /= 3;
    back_color.b /= 4;

    DrawRectangleRounded(bounds, 0.3f, 5, DARKGRAY);
    DrawRectangleRounded(back_bounds, 0.3f, 5, back_color);
    DrawRectangleRounded(control, 0.3f, 5, color);

    return changed;
}
