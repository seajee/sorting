#ifndef _SORT_H_
#define _SORT_H_

#include "list.h"

#include <stdbool.h>
#include <unistd.h>

#define PADDING 1

typedef void *(*sort_func_t)(void*);

typedef struct
{
    List list;
    int i;
    int j;
    int k;
    int s;
    bool sorted;
    bool exit;
    useconds_t sleep;
} State;

State state_init(List list);
void draw_state(State state);
void draw_sorted_animation(State *state);

void swap(int *a, int *b);
int insertion_binary_search(State state, int x, int left, int right, int *mid);

void sort_insertion(State *state);
void *thread_sort_insertion(void *state);

void sort_insertion_binary(State *state);
void *thread_sort_insertion_binary(void *state);

void sort_bubble(State *state);
void *thread_sort_bubble(void *state);

void sort_selection(State *state);
void *thread_sort_selection(void *state);

void sort_quick(State *state);
void sort_quick_recursive(State *state, int left, int right);
void *thread_sort_quick(void *state);

void sort_bogo(State *state);
void *thread_sort_bogo(void *state);

#endif // _SORT_H_
