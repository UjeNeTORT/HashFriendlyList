#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../super_list.h"
#include "list_dump.h"

int ListDump (const char * fname, const List * list, size_t err_vec)
{
    assert(fname);
    assert(list);

    char * dot_code = FormDotCode(list, err_vec);

    WriteDotCode(fname, (const char *) dot_code);

    free(dot_code);

    return 0;
}

char * FormDotCode (const List * list, size_t err_vec)
{
    assert(list);

    size_t size = list->size * 1000;

    char * dot_code = (char *) calloc(size, sizeof(char));

    int symbs = 0;

    int offset = 0;

    char * vals  = (char *) FormVals(list, size);
    char * nodes = (char *) FormNodes(list, size);
    char * edges = (char *) FormEdges(list, size);

    sprintf(dot_code, "digraph list {\n"
                      "layout=dot\n"
                      "%s"
                      "%s"
                      "%s"
                      "}\n", nodes, edges, vals);

    free(vals);
    free(nodes);
    free(edges);

    return dot_code;
}

char * FormVals (const List * list, size_t size)
{
    assert(list);

    char * vals = (char *) calloc(size, sizeof(char));
    char * vals_init = vals;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));
    char * style_init = style;

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\" %n", &symbs);

    sprintf(vals, "subgraph cluster_val {\n%n", &symbs);
    vals += symbs;

    sprintf(vals, "     val_head [ shape = box3d, %s label = \" head\n%d \"] %n", style, list->head, &symbs);
    vals += symbs;

    sprintf(vals, "     val_tail [ shape = box3d, %s label = \" tail\n%d \"] %n", style, list->tail, &symbs);
    vals += symbs;

    sprintf(vals, "     val_fre  [ shape = box3d, %s label = \" fre\n%d \"] %n", style, list->fre, &symbs);
    vals += symbs;

    sprintf(vals, "}\n%n", &symbs);
    vals += symbs;

    free(style_init);

    return vals_init;
}

char * FormNodes (const List * list, size_t size)
{
    assert(list);

    char * nodes = (char *) calloc(size, sizeof(char));
    char * nodes_init = nodes;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));
    char * style_init = style;

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\" %n", &symbs);

    sprintf(nodes, "subgraph cluster_nodes {\n%n", &symbs);
    nodes += symbs;

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        sprintf(nodes, "    node_%d [%s shape = record, label = \"{ %d | data = %d | <fnext> next = %d | <fprev> prev = %d }\"];\n%n", i, style, i, list->data[i], list->next[i], list->prev[i], &symbs);
        nodes += symbs;
    }

    sprintf(nodes, "}\n%n", &symbs);
    nodes += symbs;

    free(style_init);

    return nodes_init;
}

char * FormEdges (const List * list, size_t size)
{
    assert(list);

    char * edges = (char *) calloc(size, sizeof(char));
    char * edges_init = edges;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));
    char * style_init = style;

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\" %n", &symbs);

    sprintf(edges, "val_head -> node_%d;\n%n", list->head, &symbs);
    edges += symbs;

    sprintf(edges, "val_tail -> node_%d;\n%n", list->tail, &symbs);
    edges += symbs;

    sprintf(edges, "val_fre -> node_%d;\n%n", list->fre, &symbs);
    edges += symbs;

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        sprintf(edges, "node_%d: <fprev> -> node_%d: <fprev>;\n%n", i, list->prev[i], &symbs);
        edges += symbs;
    }

    for (int i = 0; i < list->size - 1; i++, symbs = 0)
    {
        sprintf(edges, "node_%d: <fnext> -> node_%d: <fnext>;\n%n", i, list->next[i], &symbs);
        edges += symbs;
    }

    free(style_init);

    return edges_init;
}

int WriteDotCode (const char * fname, const char * dot_code)
{
    assert(fname);
    assert(dot_code);

    FILE * fout = fopen(fname, "wb");

    int ret_code = fprintf(fout, dot_code);

    fclose(fout);

    return ret_code;
}
