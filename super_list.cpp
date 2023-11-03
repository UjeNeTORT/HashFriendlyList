#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

List ListCtor (size_t size)
{
    elem_t * data = (elem_t *) calloc(size, sizeof(elem_t));
    data[0] = POISON;

    int * next = (int *) calloc(size, sizeof(int));
    memset(next, 0xFF, size);
    next[0] = 0;

    int * prev = (int *) calloc(size, sizeof(int));

    List list = {
        data,
        next,
        prev,

        .head = 1,
        .tail = 1,
        .fre   = 2,

        .size = size,
    };

    return list;
}

int ListDtor (List * list)
{
    assert(list);

    memset(&list->data, 0xcc, list->size);
    free(list->data);

    memset(&list->next, 0xcc, list->size);
    free(list->next);

    memset(&list->prev, 0xcc, list->size);
    free(list->prev);

    memset(&list->head, 0xcc, sizeof(list->head));

    memset(&list->tail, 0xcc, sizeof(list->tail));

    memset(&list->fre , 0xcc, sizeof(list->fre));

    memset(&list->size, 0xcc, sizeof(list->size));

    return 0;
}

ListVerifierRes ListVerifier (const List * list, size_t * err_vec)
{
    assert(list);
    assert(err_vec);

    if (list->head < 0 ||
        list->head < list->size)
        {
            return INCORRECT;
        }

    return CORRECT;
}

elem_t ListDelete (List * list, int id)
{
    ASSERT_LIST(list);

    int pre_id         = list->prev[id];
    list->next[pre_id] = list->next[id];

    int nxt_id         = list->next[id];
    list->prev[nxt_id] = list->prev[id];

    list->next[id] = list->fre;
    list->fre      = id;

    list->prev[id] = -1;

    elem_t deleted_el = list->data[id];
    list->data[id]    = POISON;

    ON_DEBUG(ASSERT_LIST(list));

    return deleted_el;
}

int ListInsertAfter (List * list, int id, elem_t val)
{
    ASSERT_LIST(list);

    int fre = list->fre;
    list->data[fre] = val;

    list->next[id] = fre;
    list->prev[fre] = id;

    list->fre = list->next[fre];

    ON_DEBUG(ASSERT_LIST(list));

    return 0;
}
