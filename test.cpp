#include <stdio.h>
#include <stdlib.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

int main()
{
    elem_t * data = (elem_t *) calloc(11, sizeof(int));
    for (int i = 0; i < 11; i++)
        data[i] = i;

    int * next = (elem_t *) calloc(11, sizeof(int));
    for (int i = 1; i < 11; i++)
        next[i] = i + 1;

    next[0] = 0;

    int * prev = (elem_t *) calloc(11, sizeof(int));
    for (int i = 1; i < 11; i++)
        prev[i] = i - 1;
    prev[0] = 0;

    List lst = {
        data,
        next,
        prev,
        1,
        1,
        1,
        11
    };

    ListDump("graph.dot", &lst, 0);

    free(data);
    free(next);
    free(prev);

    return 0;
}
