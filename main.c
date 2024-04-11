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

#define COLOR_SELECTED GOLD
#define COLOR_EXCLUDED DARKGRAY

#define DEFAULT_LIST_LENGTH 50

int main(int argc, char **argv)
{
    int length = DEFAULT_LIST_LENGTH;
    if (argc > 1)
        length = atoi(argv[1]);

    SetRandomSeed(time(NULL));

    List list = list_alloc(length);
    assert(list.arr != NULL);

    State state = state_init(list);
    list_fill_random(list, 1, 100);

    sort_func_t sort_func = thread_sort_insertion;

    bool thread_working = false;
    pthread_t thread_id;

    Color color = COLOR_SELECTED;

    Rectangle button;

    float sleep = 100000.0f;
    Rectangle slider;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Font font = LoadFont("Roboto.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        button.x = screen_width / 100;
        button.y = screen_height / 100;
        button.width = screen_width / 10;
        button.height = screen_height / 12;

        slider.x = screen_width / 2;
        slider.y = screen_height / 50;
        slider.width = screen_width / 5;
        slider.height = screen_height / 32;

        state.sleep = sleep;

        BeginDrawing();
            ClearBackground(GetColor(0x202020FF));

            if ((gui_button(button, DARKGREEN, font, 1.5f, "Start") || IsKeyPressed(KEY_START))
                    && state.exit == true) {
                state = state_init(list);
                state.sleep = sleep;
                state.exit = false;
                pthread_create(&thread_id, NULL, sort_func, &state);
                thread_working = true;
            }
            button.x += button.width + 5;
            if (gui_button(button, MAROON, font, 1.5f, "Stop") || IsKeyPressed(KEY_STOP)) {
                state.exit = true;
                thread_working = false;
            }
            button.x += button.width + 5;
            if ((gui_button(button, GRAY, font, 1.5f, "Randomize") || IsKeyPressed(KEY_RANDOMIZE))
                    && state.exit == true) {
                list_fill_random(list, 1, 100);
            }

            button.x -= (button.width + 5) * 2;
            button.y += button.height + 5;
            if (sort_func == thread_sort_insertion) color = COLOR_SELECTED;
            if ((gui_button(button, color, font, 1.5f, "Insertion Sort"))
                    && state.exit == true) {
                sort_func = thread_sort_insertion;
            }
            color = COLOR_EXCLUDED;
            button.x += button.width + 5;
            if (sort_func == thread_sort_bubble) color = COLOR_SELECTED;
            if ((gui_button(button, color, font, 1.5f, "Bubble Sort"))
                    && state.exit == true) {
                sort_func = thread_sort_bubble;
            }
            color = COLOR_EXCLUDED;

            if (gui_slider(slider, MAROON, 1.0f, 100000.0f, &sleep)) {
                state.sleep = sleep;
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
