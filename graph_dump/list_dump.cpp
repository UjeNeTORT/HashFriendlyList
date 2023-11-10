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

    char * vals  = FormVals     (list, size);
    char * nodes = FormNodes    (list, size);
    char * edges = FormEdges    (list, size);

    sprintf(dot_code, "digraph list_%d {\n"
                      "rankdir = LR\n"
                      "splines = ortho\n"
                      "nodesep = 0.8\n"
                      "\tedge[minlen = 1, penwidth = 1.5];\n"
                      "%s"
                      "%s"
                      "%s"
                      "%s"
                      "}\n", rand(), nodes, edges, vals, add_info);

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

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\"");

    sprintf(vals, "subgraph cluster_val {\n%n", &symbs);
    vals += symbs;

    sprintf(vals, "     val_fre  [ shape = none, %s, fontcolor = white, label = \" fre\n%d \", image = \"cat.jpg\"];\n%n", style, list->fre, &symbs);
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

    sprintf(nodes, "subgraph cluster_nodes_%d {\n"
                   "bgcolor=\"#B5E2FA\" \n%n", rand(), &symbs);
    nodes += symbs;

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        if (list->prev[i] != -1)
        {
            sprintf(nodes, "\tnode_%d [%s shape = record, style = filled, fillcolor = \"#4CB944\", label = \" %d | data = %d | <fnext> next = %d | <fprev> prev = %d \"];\n%n", i, style, i, list->data[i], list->next[i], list->prev[i], &symbs);
            nodes += symbs;
        }
        else
        {
            sprintf(nodes, "\tnode_%d [%s shape = record, style = filled, fillcolor = \"#F5EE9E\", label = \" %d | data = %d | <fnext> next = %d | <fprev> prev = %d \"];\n%n", i, style, i, list->data[i], list->next[i], list->prev[i], &symbs);
            nodes += symbs;
        }
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

    if (list->fre != -1)
    {
        sprintf(edges, "val_fre -> node_%d;\n%n", list->fre, &symbs);
        edges += symbs;
    }

    // invisible arrows to align graph elems
    for (int i = 0; i < list->size - 1; i++) {
        sprintf(edges, "\tnode_%d -> node_%d[weight = 1000, style = invis];\n %n", i, i + 1, &symbs);
        edges += symbs;
    }

    // next arrows
    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        if (list->next[i] != -1)
        {
            if (list->prev[i] != -1)
            {
                sprintf(edges, "node_%d -> node_%d  [color = blue];\n%n", i, list->next[i], &symbs);
                edges += symbs;
            }
            else
            {
                sprintf(edges, "node_%d -> node_%d  [color = grey];\n%n", i, list->next[i], &symbs);
                edges += symbs;
            }

        }
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
