#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

static elem_t* ListDataCopy (List * list);

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

List* ListRealloc  (List * list, int new_size)
{
    ASSERT_LIST(list);

    if (new_size > list->size)
        //ListReallocUp
    if(new_size < )
    List realloced_list = ListLinear(list);





    ON_DEBUG(ASSERT_LIST(list));
}

List ListLinear (List * list)
{
    ASSERT_LIST(list);

    List linear_list = ListCtor(list->size); // create fresh list of the same size

    int next_id  = 0;
    int logic_id = 0;

    while (next_id != list->prev[0])
    {
        next_id = list->next[next_id];

        ListInsertAfter(&linear_list, logic_id, list->data[next_id]);
        logic_id++;
    }

    ON_DEBUG(ASSERT_LIST(list));

    return linear_list;
}

elem_t ListIdFind (List * list, int id)
{
    ASSERT_LIST(list);

    elem_t val = POISON;

    if (0 < id && id < list->size)
        val = list->data[id];
    else
        fprintf(stderr, "ListIdFind: invalid id %d\n", id);

    ON_DEBUG(ASSERT_LIST(list));

    return val;
}

int MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind (List * list, elem_t val)
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

int ListInsertEnd (List * list, elem_t val)
{
    ASSERT_LIST(list);

    int new_id = ListInsertAfter(list, list->prev[0], val);

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

int ListInsertBefore (List * list, int id, elem_t val)
{
    ASSERT_LIST(list);

    int new_id = ListInsertAfter(list, list->prev[id], val);

    ON_DEBUG(ASSERT_LIST(list));

    return new_id;
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

int ListValDelete (List * list, elem_t val)
{
    ASSERT_LIST(list);

    int id = MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind(list, val);

    if (id != -1)
    {
        elem_t del_val = ListIdDelete(list, id);
    }

    ON_DEBUG(ASSERT_LIST(list));

    return id;
}

ListVerifierRes ListVerifier (const List * list, size_t * err_vec)
{
    assert(err_vec);

    if (!list)
    {
        *err_vec |= 1;

        return INCORRECT;
    }

    if (list->next[0] < 0 || list->prev[0] > list->size)
    {
        *err_vec |= 2;
    }

    if (!err_vec)   return INCORRECT;

    return CORRECT;
}

static elem_t* ListDataCopy (List * list)
{
    ASSERT_LIST(list);

    elem_t * data_copy = (elem_t *) calloc(list->size, sizeof(elem_t));

    for (int i = 0; i < list->size; i++)
    {
        data_copy[i] = list->data[i];
    }

    return data_copy;
}
