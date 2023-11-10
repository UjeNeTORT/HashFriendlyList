#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include <stdio.h>
#include "../super_list.h"

typedef enum {
    LST_ERR_NO_LIST_PTR    = 1,
    LST_ERR_NO_DATA_PTR    = 2,
    LST_ERR_NO_NEXT_PTR    = 4,
    LST_ERR_NO_PREV_PTR    = 8,
    LST_ERR_HEAD_TAIL      = 16,
    LST_ERR_CHAIN          = 32,
    LST_ERR_FRE_PREV       = 64,
    LST_ERR_FRE            = 128,
} ERRORS_LIST;

const char * const DUMP_FNAME = "list_dump.svg";
const int STYLE_TAG_SIZE = 1000;

int    ListDump     (const char * fname, const List * list, size_t err_vec);
char * FormVals     (const List * list, size_t size);
char * FormNodes    (const List * list, size_t size);
char * FormEdges    (const List * list, size_t size);
char * FormSameRank (const List * list, size_t size);
char * FormDotCode  (const List * list, size_t err_vec, const char * add_info);
int    WriteDotCode (const char * fname, const char * dot_code);

char * FormAddInfo();

#endif // LIST_DUMP_H
