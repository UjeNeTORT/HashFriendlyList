#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../super_list.h"
#include "list_dump.h"

int ListDump (const char * fname, const List * list, size_t err_vec)
{
    assert(fname);
    assert(list);

    char * add_info = FormAddInfo();
    char * dot_code = FormDotCode(list, err_vec, (const char *) add_info);

    WriteDotCode(fname, (const char *) dot_code);

    free(dot_code);
    free(add_info);

    return 0;
}

char * FormDotCode (const List * list, size_t err_vec, const char * add_info)
{
    assert(list);

    size_t size = list->size * 1000;

    char * dot_code = (char *) calloc(size, sizeof(char));

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    sprintf(style, "bgcolor=\"lightblue\"\n"
                   "");

    char * vals  = FormVals     (list, size);
    char * nodes = FormNodes    (list, size);
    char * edges = FormEdges    (list, size);
    char * rank  = FormSameRank (list, size);

    sprintf(dot_code, "digraph list_%d {\n"
                      "rankdir = LR\n"
                    //   "\tsplines = ortho;\n"
                      "\tedge[minlen = 1.5, penwidth = 1.5];\n"
                      "%s"
                      "%s"
                      "%s"
                      "%s"
                      "%s"
                      "}\n", rand(), nodes, edges, vals, style, add_info);

    free(vals);
    free(nodes);
    free(edges);
    free(style);
    free(rank);

    return dot_code;
}

char * FormVals (const List * list, size_t size)
{
    assert(list);

    char * vals = (char *) calloc(size, sizeof(char));
    char * vals_init = vals;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\"");

    sprintf(vals, "subgraph cluster_val {\n%n", &symbs);
    vals += symbs;

    sprintf(vals, "     val_fre  [ shape = box3d, %s, label = \" fre\n%d \"];\n%n", style, list->fre, &symbs);
    vals += symbs;

    sprintf(vals, "}\n%n", &symbs);
    vals += symbs;

    free(style);

    return vals_init;
}

char * FormNodes (const List * list, size_t size)
{
    assert(list);

    char * nodes = (char *) calloc(size, sizeof(char));
    char * nodes_init = nodes;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\""
                   "style=rounded\n"
                   "color = green\n");

    sprintf(nodes, "subgraph cluster_nodes_%d {\n%n", rand(), &symbs);
    nodes += symbs;

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        sprintf(nodes, "    node_%d [%s shape = record, label = \" %d | data = %d | <fnext> next = %d | <fprev> prev = %d \"];\n%n", i, style, i, list->data[i], list->next[i], list->prev[i], &symbs);
        nodes += symbs;
    }

    sprintf(nodes, "}\n%n", &symbs);
    nodes += symbs;

    free(style);

    return nodes_init;
}

char * FormEdges (const List * list, size_t size)
{
    assert(list);

    char * edges = (char *) calloc(size, sizeof(char));
    char * edges_init = edges;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    // sprintf(edges, "val_head -> node_%d;\n%n", list->head, &symbs);
    // edges += symbs;

    // sprintf(edges, "val_tail -> node_%d;\n%n", list->tail, &symbs);
    // edges += symbs;

    sprintf(edges, "val_fre -> node_%d;\n%n", list->fre, &symbs);
    edges += symbs;

    for (int i = 0; i < list->size - 1; i++) {
        sprintf(edges, "\tnode_%d -> node_%d[weight = 100, style = invis];\n %n", i, i + 1, &symbs);
        edges += symbs;
    }

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        sprintf(edges, "node_%d -> node_%d  [color = blue];\n%n", i, list->next[i], &symbs);
        edges += symbs;
    }

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        if (list->prev[i] != -1)
        {
            sprintf(edges, "node_%d -> node_%d  [color = red];\n%n", i, list->prev[i], &symbs);
            edges += symbs;
        }
    }

    free(style);

    return edges_init;
}

char * FormSameRank (const List * list, size_t size)
{
    assert(list);

    char * same_rank = (char *) calloc(size, sizeof(char));
    char * same_rank_init = same_rank;

    int symbs = 0;

    sprintf(same_rank, "{rank = same; %n", &symbs);
    same_rank += symbs;

    for (int i = 1; i < list->size; i++, symbs = 0)
    {
        sprintf(same_rank, "node_%d; %n", i,&symbs);
        same_rank += symbs;
    }

    sprintf(same_rank, "}\n%n", &symbs);
    same_rank += symbs;

    sprintf(same_rank, "{rank = same; node_0; node_add_info; val_head; val_tail; val_fre; }\n%n", &symbs);
    same_rank += symbs;

    return same_rank_init;
}

int WriteDotCode (const char * fname, const char * dot_code)
{
    assert(fname);
    assert(dot_code);

    FILE * fout = fopen(fname, "wb");

    int ret_code = fprintf(fout, "%s", dot_code);

    fclose(fout);

    return ret_code;
}

char * FormAddInfo()
{
    char * add_info = (char *) calloc(1000, sizeof(char));

    time_t t = time(NULL);

    tm * loc_time = localtime(&t);

    sprintf(add_info, "subgraph cluster_add_info_%d{\n"
                      "node_add_info [shape = plaintext, label = \" %s \"];\n"
                      "}\n", rand(), asctime(loc_time));

    return add_info;
}
