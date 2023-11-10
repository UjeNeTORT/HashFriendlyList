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

static ListSwapRes     ListElemSwap  (List * list, int id_1, int id_2);
static ListReallocRes  ListReallocUp (List * list, int new_size);

List ListCtor (int size)
{
    elem_t* data = (elem_t *) calloc(size, sizeof(elem_t));
    int*    next = (int *)    calloc(size, sizeof(int));
    int*    prev = (int *)    calloc(size, sizeof(int));

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

    next[size - 1] = -1; // no next node after the last one

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
    VERIFY_LIST(list_src);

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

    ON_DEBUG(VERIFY_LIST(list_dst));

    return CPY_NO_ERR;
}

ListReallocRes ListRealloc  (List * list, int new_size)
{
    VERIFY_LIST(list);

    ListReallocRes ret_val = REALLC_NO_ERR;

    #ifdef LINEARIZATION
        ListMakeLinear(list);
    #endif // LINEARIZATION

    if (new_size > list->size)
    {
        ret_val = ListReallocUp(list, new_size);
    }
    else if (new_size < list->size)
    {
        ret_val = REALLC_FORBIDDEN;
    }
    else if (new_size == list->size)
    {
        ret_val = REALLC_FORBIDDEN;
    }

    ON_DEBUG(VERIFY_LIST(list));

    return ret_val;
}

static ListReallocRes
ListReallocUp (List * list, int new_size)
{
    VERIFY_LIST(list);

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

    int id_free = list->fre;

    while (NEXT(id_free) != -1)
    {
        id_free = NEXT(id_free);
    }

    for (int i = id_free; i < new_size; i++)
    {
        NEXT(i) = i + 1;
        DATA(i) = POISON;
        PREV(i) = -1;
    }

    NEXT(new_size - 1) = -1; // last element has no next

    list->size = new_size;

    ON_DEBUG(VERIFY_LIST(list));

    return REALLC_NO_ERR;
}

int ListMakeLinear (List * list)
{
    VERIFY_LIST(list);

    List linear_list = ListCtor(list->size); //? create fresh list of the same size

    int next_id  = 0;
    int logic_id = 0;

    while (next_id != list->prev[0])
    {
        next_id = NEXT(next_id);

        ListInsertAfter(&linear_list, logic_id, DATA(next_id));

        logic_id++;
    }

    ListDtor(list); //? good solution?

    list = &linear_list;

    ON_DEBUG(VERIFY_LIST(list));

    return 0;
}

elem_t ListIdFind (List * list, int id)
{
    VERIFY_LIST(list);

    elem_t val = POISON;

    if (0 < id && id < list->size)
        val = DATA(id);
    else
        fprintf(stderr, "ListIdFind: invalid id %d\n", id);

    ON_DEBUG(VERIFY_LIST(list));

    return val;
}

int MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind (List * list, elem_t val)
{
    VERIFY_LIST(list);

    int id = -1; // not found

    for (int i = NEXT(0); i != 0; i = NEXT(i))
    {
        if (DATA(i) == val)
        {
            id = i;
            break;
        }
    }

    ON_DEBUG(VERIFY_LIST(list));

    return id;
}

int ListInsertStart (List * list, elem_t val)
{
    VERIFY_LIST(list);

    int new_id = ListInsertAfter(list, 0, val);

    ON_DEBUG(VERIFY_LIST(list));

    return new_id;  // where inserted value is
}

int ListInsertEnd (List * list, elem_t val)
{
    VERIFY_LIST(list);

    int new_id = ListInsertAfter(list, PREV(0), val);

    ON_DEBUG(VERIFY_LIST(list));

    return new_id;  // where inserted value is
}

int ListInsertAfter (List * list, int id, elem_t val)
{
    VERIFY_LIST(list);

    int new_id = list->fre;
    list->fre = NEXT(new_id);

    DATA(new_id) = val;

    int old_nxt = NEXT(id);

    NEXT(id) = new_id;
    NEXT(new_id) = old_nxt;

    PREV(new_id) = PREV(old_nxt);
    PREV(old_nxt) = new_id;

    ON_DEBUG(VERIFY_LIST(list));

    return new_id; // where inserted value is
}

int ListInsertBefore (List * list, int id, elem_t val)
{
    VERIFY_LIST(list);

    int new_id = ListInsertAfter(list, PREV(id), val);

    ON_DEBUG(VERIFY_LIST(list));

    return new_id;
}

elem_t ListIdDelete (List * list, int id)
{
    VERIFY_LIST(list);

    int prev = PREV(id);
    int next = NEXT(id);

    NEXT(prev) = next;
    PREV(next) = prev;

    elem_t deleted_el = DATA(id);
    DATA(id) = POISON;

    PREV(id) = -1;
    NEXT(id) = list->fre;
    list->fre = id;

    ON_DEBUG(VERIFY_LIST(list));

    return deleted_el;
}

int ListValDelete (List * list, elem_t val)
{
    VERIFY_LIST(list);

    int id = MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind(list, val);

    if (id != -1)
    {
        elem_t del_val = ListIdDelete(list, id);
    }

    ON_DEBUG(VERIFY_LIST(list));

    return id;
}

//! not finished
size_t ListVerifier (const List * list)
{
    size_t err_vec = 0;

    if (!list)
    {
        err_vec |= 1;

        return err_vec;
    }

    if (NEXT(0) < 0 || PREV(0) > list->size)
    {
        err_vec |= 2;
    }



    return err_vec;
}
