#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

#define NEXT(index) list->next[(index)]
#define PREV(index) list->prev[(index)]
#define DATA(index) list->data[(index)]


typedef enum {
    SWP_NO_ERR = 0,
    SWP_ERR    = 1,
} ListSwapRes;

static elem_t*         ListDataCopy  (List * list);
static ListSwapRes     ListElemSwap  (List * list, int id_1, int id_2);
static ListReallocRes  ListReallocUp (List * list, int new_size);

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

ListCopyRes ListCopy (List * list_dst, const List * list_src)
{
    ASSERT_LIST(list_src);

    if (!memcpy(list_dst->data, list_src->data, list_src->size * sizeof(elem_t)))
    {
        return CPY_ERR_MEMCPY;
    }

    if (!memcpy(list_dst->next, list_src->next, list_src->size * sizeof(int)))
    {
        return CPY_ERR_MEMCPY;
    }

    if (!memcpy(list_dst->prev, list_src->prev, list_src->size * sizeof(int)))
    {
        return CPY_ERR_MEMCPY;
    }

    list_dst->fre = list_src->fre;

    ON_DEBUG(ASSERT_LIST(list_dst));

    return CPY_NO_ERR;
}

//! not finished
// user should understand that realloc calls ListMakeLinear inside
ListReallocRes ListRealloc  (List * list, int new_size)
{
    ASSERT_LIST(list);

    ListReallocRes ret_val = REALLC_NO_ERR;

    if (new_size > list->size)
    {
        ListReallocRes ret_val = ListReallocUp(list, new_size);
    }
    else if (new_size < list->size)
    {
        ListMakeLinear(list); // * reallocation down is not yet implemented so it just makes list linear and returns "forbidden"

        ret_val = REALLC_FORBIDDEN;
    }
    else if (new_size == list->size)
    {
        ListMakeLinear(list);

        ret_val = REALLC_FORBIDDEN;
    }

    ON_DEBUG(ASSERT_LIST(list));

    return ret_val;
}

//! not finished
static ListReallocRes
ListReallocUp (List * list, int new_size)
{
    ASSERT_LIST(list);

    if (new_size <= list->size)
    {
        return REALLC_ERR;
    }

    list->data = (elem_t *) realloc(list->data, new_size * sizeof(elem_t));
    if (!list->data)
    {
        return REALLC_ERR;
    }

    list->next = (int *) realloc(list->next, new_size * sizeof(int));
    if (!list->next)
    {
        return REALLC_ERR;
    }

    list->prev = (int *) realloc(list->prev, new_size * sizeof(int));
    if (!list->prev)
    {
        return REALLC_ERR;
    }


    for (int i = 0; i < new_size; i++)
    {
        ;
    }

    list->size = new_size;

    ON_DEBUG(ASSERT_LIST(list));

    return REALLC_NO_ERR;
}

// ! not finished
int ListMakeLinear (List * list)
{
    ASSERT_LIST(list);

    int curr_el = 0;
    int logic_id = 1;
    int tail     = list->prev[0];

    int * next_tmp = (int *) calloc(list->size, sizeof(int));

    memcpy(next_tmp, list->next, list->size * sizeof(int));

    ListElemSwap(list, 2, 4);
    return 0;
    while (*next_tmp != tail)
    {

        ListElemSwap(list, *next_tmp, logic_id);

        next_tmp++;
    }

    free(next_tmp);

    ON_DEBUG(ASSERT_LIST(list));

    return 0;
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


#define NEXT(indx) list->next[indx]

// phys_indx
int ListInsertAfter (List * list, int id, elem_t val)
{
    ASSERT_LIST(list);

    int new_id = list->fre;
    list->fre = list->next[new_id];

    list->data[new_id] = val;

    // old_next
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

//! not finished
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

    // return !!err_vec;
    if (!err_vec)   return INCORRECT;

    return CORRECT;
}

// ! not finished
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

static ListSwapRes
ListElemSwap  (List * list, int id_1, int id_2)
{
    // no list checks as they would slow down performance of list

    if (id_1 == 0 || id_2 == 0)
    {
        fprintf(stderr, "ListElemSwap: elem id == 0 - forbidden\n");

        return SWP_ERR;
    }

    if (list->prev[id_1] == -1 || list->prev[id_2] == -1)
    {
        fprintf(stderr, "ListElemSwap: one of ids \"points\" to a non-used list element\n");

        return SWP_ERR;
    }

    if (id_1 == id_2)
    {
        return SWP_NO_ERR;
    }

    if (PREV(id_2) == id_1)
    {
        int temp_id = id_1;
        id_1 = id_2;
        id_2 = temp_id;
    }

    NEXT(PREV(id_2)) = id_1;
    PREV(NEXT(id_1)) = id_2;

    PREV(id_1) = PREV(id_2);
    NEXT(id_2) = NEXT(id_1);

    NEXT(id_1) = id_2;
    PREV(id_2) = id_1;

    elem_t temp_data = DATA(id_1);
    DATA(id_1) = DATA(id_2);
    DATA(id_2) = temp_data;

    return SWP_NO_ERR;
}
