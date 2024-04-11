#ifndef _GUI_H_
#define _GUI_H_

#include <raylib.h>

bool gui_button(Rectangle bounds, Color color, Font font, float font_scale, const char *text);
bool gui_slider(Rectangle bounds, Color color, float min, float max, float *value);

#endif // _GUI_H_
