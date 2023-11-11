#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "super_list.h"
#include "graph_dump/list_dump.h"

int main()
{
    List lst_1 = ListCtor(10);

    InsertEndList(&lst_1, 111);
    InsertEndList(&lst_1, 222);
    InsertEndList(&lst_1, 333);
    InsertBeginList(&lst_1, 444);
    InsertEndList(&lst_1, 555);
    InsertEndList(&lst_1, 666);
    InsertEndList(&lst_1, 1000);
    InsertEndList(&lst_1, 777);
    printf("%d\n", FindIdList(&lst_1, 3));
    // ListIdDelete(&lst_1, 3);

    ListDump("graph.dot", &lst_1, 0, DEBUG_INFO(&lst_1));

    ListDtor(&lst_1);

    return 0;
}
