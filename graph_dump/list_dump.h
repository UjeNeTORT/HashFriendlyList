#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include <stdio.h>
#include "../super_list.h"

const char * const DUMP_FNAME = "list_dump.svg";
const int STYLE_TAG_SIZE = 1000;

int    ListDump     (const char * fname, const List * list, size_t err_vec);
char * FormVals     (const List * list, size_t size);
char * FormNodes    (const List * list, size_t size);
char * FormEdges    (const List * list, size_t size);
char * FormDotCode  (const List * list, size_t err_vec);
int    WriteDotCode (const char * fname, const char * dot_code);

#endif // LIST_DUMP_H
