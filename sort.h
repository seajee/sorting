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
    bool sorted;
    bool exit;
    useconds_t sleep;
} State;

State state_init(List list);
void draw_state(State state);

void swap(int *a, int *b);

void sort_insertion(State *state);
void *thread_sort_insertion(void *state);

void sort_bubble(State *state);
void *thread_sort_bubble(void *state);

void sort_selection(State *state);
void *thread_sort_selection(void *state);

#endif // _SORT_H_
