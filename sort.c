#include "sort.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>

State state_init(List list)
{
    State state = {0};
    state.list = list;
    state.i = -1;
    state.j = -1;
    state.sorted = false;
    state.exit = true;
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

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
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

            usleep(state->sleep);
        }
    }

    *i = -1;
    *j = -1;
    state->sorted = true;
    state->exit = true;
}

void *thread_sort_insertion(void *state)
{
    sort_insertion((State*)state);
    return NULL;
}

void sort_bubble(State *state)
{
    assert(state->list.arr != NULL);

    List *list = &state->list;
    int *i = &state->i;
    int *j = &state->j;

    bool swapped = false;
    int n = list->length;

    do {
        swapped = false;

        for (*i = 1; *i < n; ++*i) {
            if (state->exit) {
                *i = -1;
                *j = -1;
                return;
            }

            *j = *i - 1;
            if (list->arr[*i - 1] > list->arr[*i]) {
                swap(&list->arr[*i - 1], &list->arr[*i]);
                swapped = true;
            }

            usleep(state->sleep);
        }
        --n;
    } while (swapped == true);

    *i = -1;
    *j = -1;
    state->sorted = true;
    state->exit = true;
}

void *thread_sort_bubble(void *state)
{
    sort_bubble((State*)state);
    return NULL;
}
