#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <raylib.h>

#include "list.h"
#include "sort.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Sorting"

int main(void)
{
    SetRandomSeed(time(NULL));

    List list = list_alloc(200);
    assert(list.arr != NULL);

    State state = state_init(list);
    list_fill_random(list, 1, 3000);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_sort_bubble, &state);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R) && state.exit == true) {
            list_fill_random(list, 1, 100);
        }
        if (IsKeyPressed(KEY_A)) {
            state.exit = true;
        }
        if (IsKeyPressed(KEY_ENTER) && state.exit == true) {
            state = state_init(list);
            pthread_create(&thread_id, NULL, thread_sort_bubble, &state);
        }

        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            DrawText("[R]: Generate new list\n\n[ENTER]: Start sorting\n\n[A]: Abort sorting", 10, 10, 30, WHITE);
            draw_state(state);
        EndDrawing();
    }

    CloseWindow();
    state.exit = true;
    pthread_join(thread_id, NULL);
    list_free(list);

    return 0;
}
