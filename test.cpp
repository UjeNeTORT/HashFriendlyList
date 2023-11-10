#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "microhashlib/my_hash.h"

#include "super_list.h"
#include "graph_dump/list_dump.h"

int main()
{
    List lst_1 = ListCtor(10);


    ListInsertEnd(&lst_1, 111);
    ListInsertEnd(&lst_1, 222);
    ListInsertEnd(&lst_1, 333);
    ListInsertBegin(&lst_1, 444);
    ListInsertEnd(&lst_1, 555);
    ListInsertEnd(&lst_1, 666);
    ListInsertEnd(&lst_1, 1000);
    ListInsertEnd(&lst_1, 777);
    ListIdDelete(&lst_1, 3);

    ListDump("graph.dot", &lst_1, 0);

    ListDtor(&lst_1);

    return 0;
}
