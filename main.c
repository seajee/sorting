#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <raylib.h>
#include <raymath.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Sorting"
#define PADDING 1
#define SLEEP 1000 * 5 // microseconds

typedef struct
{
    int *arr;
    int length;
} List;

typedef struct
{
    List list;
    int i;
    int j;
    bool sorted;
    bool exit;
} State;

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

List list_alloc(size_t length)
{
    List list = {0};
    list.arr = (int*)malloc(sizeof(int) * length);
    list.length = length;
    return list;
}

void list_free(List list)
{
    free(list.arr);
}

int list_max(List list)
{
    int max = INT_MIN;
    for (int i = 0; i < list.length; ++i) {
        if (list.arr[i] > max) {
            max = list.arr[i];
        }
    }
    return max;
}

void list_fill_random(List list, int min, int max)
{
    for (int i = 0; i < list.length; ++i) {
        list.arr[i] = GetRandomValue(min, max);
    }
}

State state_init(List list)
{
    State state = {0};
    state.list = list;
    state.i = -1;
    state.j = -1;
    state.sorted = false;
    state.exit = false;
    return state;
}

void draw_state(State state)
{
    List list = state.list;
    assert(list.arr != NULL);
    assert(list.length != 0);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float rw = (float)sw / list.length;
    int max = list_max(list);
    float max_height = sh / 2;

    Vector2 pos = Vector2Zero();
    Vector2 size = Vector2Zero();
    Color color = WHITE;

    for (int i = 0; i < list.length; ++i) {
        size.x = rw - PADDING;
        size.y = list.arr[i] * max_height / max;
        pos.x = rw * i + PADDING;
        pos.y = sh - size.y;

        color = WHITE;
        if (i == state.i)
            color = BLUE;
        if (i == state.j)
            color = RED;

        DrawRectangleV(pos, size, color);
    }
}

void sort_insertion(State *state)
{
    assert(state->list.arr != NULL);

    List *list = &state->list;
    int *i = &state->i;
    int *j = &state->j;

    for (*i = 1; *i < list->length; ++*i) {
        for (*j = *i; *j > 0; --*j) {
            if (state->exit) {
                *i = -1;
                *j = -1;
                return;
            }

            if (list->arr[*j] < list->arr[*j - 1]) {
                swap(&list->arr[*j], &list->arr[*j - 1]);
            } else {
                break;
            }
            usleep(SLEEP);
        }
    }

    *i = -1;
    *j = -1;
    state->sorted = true;
}

void *thread_sort_insertion(void *state)
{
    sort_insertion((State*)state);
    return NULL;
}

int main(void)
{
    SetRandomSeed(time(NULL));

    List list = list_alloc(200);
    assert(list.arr != NULL);

    State state = state_init(list);
    list_fill_random(list, 1, 3000);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_sort_insertion, &state);

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
            pthread_create(&thread_id, NULL, thread_sort_insertion, &state);
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
