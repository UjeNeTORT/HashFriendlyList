#ifndef SUPER_LIST_H
#define SUPER_LIST_H

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

    int head;
    int tail;
    int fre;

    size_t size;
};

ListVerifierRes ListVerifier (const List * list, size_t * err_vec);
List            ListCtor     (size_t size);
int             ListDtor     (List * list);

int    ListInsertAfter (List * list, int id, elem_t val);
elem_t ListDelete      (List * list, int id);

#endif // SUPER_LIST_H
