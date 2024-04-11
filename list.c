#include "list.h"

#include <stdlib.h>
#include <limits.h>
#include <raylib.h>

List list_alloc(size_t length)
{
    List list = {0};
    list.arr = (int*)malloc(sizeof(int) * length);
    list.length = length;
    return list;
}

void list_realloc(List list, size_t length)
{
    if (list.arr == NULL)
        return;

    list.arr = (int*)realloc(list.arr, length);
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
