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
    CORRECT = 0,
    INCORRECT = 1,
} ListVerifierRes;

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
 * @brief iterate through the list and find "val". If found return id of "val", else return -1;
 *
 * @param list list
 * @param val  value, id of which is to be returned
 *
 * @return -1 if "val" not found in the list
 * @return id of "val" in the list if found
*/
int    ListValFind     (List * list, elem_t val);

/**
 * @brief !!! function name is to make user hate this function and to teach them to store indexes
 * @brief it is not how i usually name functions, it is some sort of joke !!!
 * @brief iterate through list and find "val", if found - delete and return its id, else - return -1
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned
 *
 * @return former id of deleted "val" if val found
 * @return -1 if "val" not found in the list
 *
*/
int    MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValDelete (List * list, elem_t val);

/**
 * @brief insert "val" in the beginning of the list (to pos fre)
 *
 * @param list list
 * @param val  value which is to be inserted in the beginning of list
 *
 * @return -1 if no free place in the list (no more reallocs we can do)
 * @return id of "val" in the list
*/
int    ListInsertStart (List * list, elem_t val);

/**
 * @brief insert "val" after value with id = "id"
 *
 * @param list list
 * @param id   id of element after which  "val" is to be inserted
 * @param val  value which is to be inserted after "id"
 *
 * @return -1 if no free space in the list (no more reallocs we can do)
 * @return id of new element "val" in the list
*/
int    ListInsertAfter (List * list, int id, elem_t val);

/**
 * @brief iterate through the list and find "val". If "val" found - delete it and return itd id, else - return -1
 *
 * @param list list
 * @param val  value to be deleted
 *
 * @return -1 if "val" not found in the list
 * @return former id of "val" in the list if found
*/
int    ListValDelete   (List * list, int val);

/**
 * @brief delete and return value with id = "id".
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned (if found)
 *
 * @return -1 if "val" not found in the list
 * @return id of "val" in the list if found
*/
elem_t ListIdDelete    (List * list, int id);

#endif // SUPER_LIST_H
