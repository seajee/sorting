#ifndef _LIST_H_
#define _LIST_H_

#include <stddef.h>

typedef struct
{
    int *arr;
    int length;
} List;

List list_alloc(size_t length);
void list_free(List list);
int list_max(List list);
void list_fill_random(List list, int min, int max);

#endif // _LIST_H_
