#include "gui.h"

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
