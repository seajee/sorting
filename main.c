#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <raylib.h>

#include "list.h"
#include "sort.h"
#include "gui.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Sorting"

#define KEY_START KEY_ENTER
#define KEY_STOP KEY_BACKSPACE
#define KEY_RANDOMIZE KEY_R

int main(void)
{
    SetRandomSeed(time(NULL));

    List list = list_alloc(30);
    assert(list.arr != NULL);

    State state = state_init(list);
    list_fill_random(list, 1, 100);

    bool thread_working = false;
    pthread_t thread_id;

    Rectangle button_pos;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Font font = LoadFont("Roboto.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        button_pos.x = screen_width / 100;
        button_pos.y = screen_height / 100;
        button_pos.width = screen_width / 10;
        button_pos.height = screen_height / 12;

        BeginDrawing();
            ClearBackground(GetColor(0x202020FF));

            if ((gui_button(button_pos, DARKGREEN, font, 1.5f, "Start") || IsKeyPressed(KEY_START))
                    && state.exit == true) {
                state = state_init(list);
                state.exit = false;
                pthread_create(&thread_id, NULL, thread_sort_bubble, &state);
                thread_working = true;
            }
            button_pos.x += button_pos.width + 5;
            if (gui_button(button_pos, MAROON, font, 1.5f, "Stop") || IsKeyPressed(KEY_STOP)) {
                state.exit = true;
                thread_working = false;
            }
            button_pos.x += button_pos.width + 5;
            if ((gui_button(button_pos, GRAY, font, 1.5f, "Randomize") || IsKeyPressed(KEY_RANDOMIZE))
                    && state.exit == true) {
                list_fill_random(list, 1, 100);
            }

            draw_state(state);
        EndDrawing();
    }

    CloseWindow();
    state.exit = true;
    if (thread_working)
        pthread_join(thread_id, NULL);
    list_free(list);

    return 0;
}
