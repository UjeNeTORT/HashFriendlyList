#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

List ListCtor (int size)
{
    elem_t * data = (elem_t *) calloc(size, sizeof(elem_t));
    int * next = (int *) calloc(size, sizeof(int));
    int * prev = (int *) calloc(size, sizeof(int));

    List list = {
        data,
        next,
        prev,

        .fre  = 1,

        .size = size,
    };

    data[0] = POISON;
    next[0] = 0; // head
    prev[0] = 0; // tail

    for (int i = 1; i < size; i++)
    {
        data[i] = POISON;
        next[i] = i + 1;
        prev[i] = -1;
    }

    return list;
}

// 0 0 0
// 0 0 1
// 1 0 -4
// 0 5 10

int ListDtor (List * list)
{
    assert(list);

    free(list->data);

    free(list->next);

    free(list->prev);

    return 0;
}

int ListInsertStart (List * list, elem_t val)
{
    ASSERT_LIST(list);

    int new_id = ListInsertAfter(list, 0, val);

    ON_DEBUG(ASSERT_LIST(list));

    return new_id;  // where inserted value is
}

int ListInsertAfter (List * list, int id, elem_t val)
{
    ASSERT_LIST(list);

    int new_id = list->fre;
    list->fre = list->next[new_id];

    list->data[new_id] = val;

    int old_nxt = list->next[id];

    list->next[id] = new_id;
    list->next[new_id] = old_nxt;

    list->prev[new_id] = list->prev[old_nxt];
    list->prev[old_nxt] = new_id;

    ON_DEBUG(ASSERT_LIST(list));

    return new_id; // where inserted value is
}

elem_t ListElemDelete (List * list, int id)
{
    ASSERT_LIST(list);

    elem_t deleted_el = 0;

    ON_DEBUG(ASSERT_LIST(list));

    return deleted_el;
}

ListVerifierRes ListVerifier (const List * list, size_t * err_vec)
{
    assert(list);
    assert(err_vec);

    if (list->next[0] < 0 ||
        list->prev[0] > list->size)
        {
            return INCORRECT;
        }

    return CORRECT;
}
