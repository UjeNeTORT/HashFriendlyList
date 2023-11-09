#ifndef SUPER_LIST_H
#define SUPER_LIST_H

#include <stdio.h>
#include "microhashlib/my_hash.h"

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__;
#else
    #define ON_DEBUG(...)
#endif // DEBUG

#define ASSERT_LIST(list) {                                          \
    size_t err_vec = 0;                                              \
    if (ListVerifier((const List *) list, &err_vec) != CORRECT)      \
    {                                                                \
        fprintf(stderr, "List corrupted\n");                         \
        abort();                                                     \
    }                                                                \
}                                                                    \

const int POISON = 0xD00D1E;

typedef int elem_t;

typedef enum {
    CORRECT   = 0,
    INCORRECT = 1,
} ListVerifierRes;

typedef enum {
    CPY_NO_ERR     = 0,
    CPY_ERR_MEMCPY = 1,
} ListCopyRes;

typedef enum {
    REALLC_NO_ERR    = 0,
    REALLC_FORBIDDEN = 1,
    REALLC_ERR       = 2,
} ListReallocRes;

struct List {

    elem_t * data;
    int * next;
    int * prev;

    int fre;

    int size;
};

ListVerifierRes ListVerifier (const List * list, size_t * err_vec);
List            ListCtor     (int size);
int             ListDtor     (List * list);

/**
 * @brief copy everything from list_src to list_dst
 *
 * @details container for list_dst is to be created manually outside the function
 *
 * @param list_dst destination list to which we copy
 * @param list_src source list from which we copy
 *
 * @return ListCopyRes (0 - if ok, not 0 - if there were errors inside)
*/
ListCopyRes     ListCopy     (List * list_dst, const List * list_src);

ListReallocRes  ListRealloc   (List * list, int new_size);

int             ListMakeLinear   (List * list); // if user needs to store both linear and non linear list, they should make copy by themselves

/**
 * @brief get value stored in list->data[id]
 *
 * @param list list
 * @param id   index of val
 *
 * @return value stored in list->data[id]
 * @return POISON if "id" stores nothing or "id" is invalid
*/
elem_t ListIdFind (List * list, int id);

/**
 * @brief !!! function name is to make user hate this function and to teach them to store indexes
 * @brief it is not how i name functions, it is a joke !!!
 *
 * @brief iterate through the list and find "val". If found return id of "val", else return -1;
 *
 * @param list list
 * @param val  value, id of which is to be returned
 *
 * @return id of "val" in the list if found
 * @return -1 if "val" not found in the list
*/
int    MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind     (List * list, elem_t val);

/**
 * @brief insert "val" in the beginning of the list
 *
 * @param list list
 * @param val  value which is to be inserted in the beginning of list
 *
 * @return id of "val" in the list
 * @return -1 if no free place in the list (no more reallocs we can do)
*/
int    ListInsertStart (List * list, elem_t val);

/**
 * @brief insert "val" in the end of the list
 *
 * @param list list
 * @param val  value which is to be inserted in the end of the list
 *
 * @return id of "val" in the list
 * @return -1 if no free place in the list (no more reallocs we can do)
*/
int    ListInsertEnd (List * list, elem_t val);

/**
 * @brief insert "val" after value with id = "id"
 *
 * @param list list
 * @param id   id of element after which  "val" is to be inserted
 * @param val  value which is to be inserted after "id"
 *
 * @return id of new element "val" in the list
 * @return -1 if no free space in the list (no more reallocs we can do)
*/
int    ListInsertAfter (List * list, int id, elem_t val);

/**
 * @brief insert "val" before value with id = "id"
 *
 * @param list list
 * @param id   id of element before which  "val" is to be inserted
 * @param val  value which is to be inserted before "id"
 *
 * @return id of new element "val" in the list
 * @return -1 if no free space in the list (no more reallocs we can do)
*/
int    ListInsertBefore (List * list, int id, elem_t val);

/**
 * @brief delete and return value with id = "id".
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned (if found)
 *
 * @return id of "val" in the list if found
 * @return -1 if "val" not found in the list
*/
elem_t ListIdDelete    (List * list, int id);

/**
 * @brief iterate through list and find "val", if found - delete and return its id, else - return -1
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned
 *
 * @return former id of deleted "val" if val found
 * @return -1 if "val" not found in the list
 *
*/
int    ListValDelete (List * list, elem_t val);

#endif // SUPER_LIST_H
