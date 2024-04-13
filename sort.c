#include "sort.h"

#include <assert.h>
#include <limits.h>
#include <raylib.h>
#include <raymath.h>

State state_init(List list)
{
    State state = {0};
    state.list = list;
    state.i = -1;
    state.j = -1;
    state.k = -1;
    state.s = -1;
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
        if (i == state.k)
            color = YELLOW;

        DrawRectangleV(pos, size, color);
    }
}

void draw_sorted_animation(State *state)
{
    List *list = &state->list;
    assert(list->arr != NULL);
    assert(list->length != 0);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float rw = (float)sw / list->length;
    int max = list_max(*list);
    float max_height = sh / 2;

    Vector2 pos = Vector2Zero();
    Vector2 size = Vector2Zero();
    Color color = WHITE;

    for (int i = 0; i < list->length; ++i) {
        size.x = rw - PADDING;
        size.y = list->arr[i] * max_height / max;
        pos.x = rw * i + PADDING;
        pos.y = sh - size.y;

        color = WHITE;
        if (state->s >= i) {
            color = GREEN;
        }

        DrawRectangleV(pos, size, color);
    }

    ++state->s;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int insertion_binary_search(State state, int x, int left, int right, int *mid)
{
    int *a = state.list.arr;

    while (left <= right) {
        if (state.exit)
            return INT_MAX;

        *mid = left + (right - left) / 2;

        if (x == a[*mid])
            return *mid + 1;
        else if (x > a[*mid])
            left = *mid + 1;
        else
            right = *mid - 1;

        usleep(state.sleep);
    }

    return left;
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

void sort_insertion_binary(State *state)
{
    assert(state->list.arr != NULL);

    List *list = &state->list;
    int *i = &state->i;
    int *j = &state->j;
    int *k = &state->k;
    int pos;
    int n;

    for (*i = 1; *i < list->length; ++*i) {
        *j = *i - 1;
        n = list->arr[*i];
        pos = insertion_binary_search(*state, n, 0, *j, k);

        while (*j >= pos) {
            if (state->exit) {
                *i = -1;
                *j = -1;
                *k = -1;
                state->exit = true;
                return;
            }

            list->arr[*j + 1] = list->arr[*j];
            --*j;

        }
        list->arr[*j + 1] = n;

        usleep(state->sleep);
    }

    *i = -1;
    *j = -1;
    *k = -1;
    state->sorted = true;
    state->exit = true;
}

void *thread_sort_insertion_binary(void *state)
{
    sort_insertion_binary((State*)state);
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

void sort_selection(State *state)
{
    assert(state->list.arr != NULL);

    List *list = &state->list;
    int *i = &state->i;
    int *j = &state->j;

    for (*i = 0; *i < list->length - 1; ++*i) {
        int min = *i;

        for (*j = *i + 1; *j < list->length; ++*j) {
            if (state->exit) {
                *i = -1;
                *j = -1;
                return;
            }

            if (list->arr[*j] < list->arr[min]) {
                min = *j;
            }

            usleep(state->sleep);
        }

        if (*i != min)
            swap(&list->arr[*i], &list->arr[min]);
    }

    *i = -1;
    *j = -1;
    state->sorted = true;
    state->exit = true;
}

void *thread_sort_selection(void *state)
{
    sort_selection((State*)state);
    return NULL;
}

void sort_quick(State *state)
{
    assert(state->list.arr != NULL);
    sort_quick_recursive(state, 0, state->list.length - 1);

    if (!state->exit)
        state->sorted = true;
    state->i = -1;
    state->j = -1;
    state->k = -1;
    state->exit = true;
}

void sort_quick_recursive(State *state, int left, int right)
{
    if (left >= right) {
        return;
    }

    List *list = &state->list;
    int *sx = &state->i;
    int *dx = &state->j;
    int *pivot = &state->k;

    *sx = left;
    *dx = right - 1;
    *pivot = right;

    while (*sx <= *dx) {
        if (state->exit) {
            return;
        }

        if (list->arr[*sx] <= list->arr[*pivot]) {
            ++*sx;
        } else {
            swap(&list->arr[*sx], &list->arr[*dx]);
            --*dx;
        }

        if (*dx < right - 1) {
            swap(&list->arr[*dx + 1], &list->arr[*pivot]);
            *pivot = *dx + 1;
        }

        usleep(state->sleep);
    }

    sort_quick_recursive(state, left, *pivot - 1);
    sort_quick_recursive(state, *pivot + 1, right);
}

void *thread_sort_quick(void *state)
{
    sort_quick((State*)state);
    return NULL;
}
