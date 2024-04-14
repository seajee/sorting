#ifndef _LIST_H_
#define _LIST_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    int *arr;
    int length;
} List;

List list_alloc(size_t length);
void list_realloc(List list, size_t length);
void list_free(List list);
int list_max(List list);
void list_fill_random(List list, int min, int max);
bool list_is_sorted(List list);

#endif // _LIST_H_
