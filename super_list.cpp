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

int ListDtor (List * list)
{
    assert(list);

    free(list->data);

    free(list->next);

    free(list->prev);

    return 0;
}


int ListValFind (List * list, elem_t val)
{
    ASSERT_LIST(list);

    int id = -1; // not found

    for (int i = list->next[0]; i != 0; i = list->next[i])
    {
        if (list->data[i] == val)
        {
            id = i;
            break;
        }
    }

    ON_DEBUG(ASSERT_LIST(list));

    return id;
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

elem_t ListValDelete   (List * list, int val)
{

}

elem_t ListIdDelete (List * list, int id)
{
    ASSERT_LIST(list);

    int prev = list->prev[id];
    int next = list->next[id];

    list->next[prev] = next;
    list->prev[next] = prev;

    elem_t deleted_el = list->data[id];
    list->data[id] = POISON;

    list->prev[id] = -1;
    list->next[id] = list->fre;
    list->fre = id;

    ON_DEBUG(ASSERT_LIST(list));

    return deleted_el;
}

//! unfinished
int MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListFindVal (List * list, elem_t val)
{
    assert(list);


    return 0;
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
