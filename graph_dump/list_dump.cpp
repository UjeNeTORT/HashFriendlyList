#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int DEFAULT_BUF_SIZE = 1000;

#include "../super_list.h"
#include "list_dump.h"

int ListDump (const char * fname, const List * list, size_t err_vec)
{
    assert(fname);
    assert(list);

    char * add_info = CreateAddInfo(err_vec);
    char * dot_code = CreateDotCode(list, (const char *) add_info);

    WriteDotCode(fname, (const char *) dot_code);

    free(dot_code);
    free(add_info);

    return 0;
}

char * CreateDotCode (const List * list, const char * add_info)
{
    assert(list);

    size_t size = DEFAULT_BUF_SIZE * 1000;

    char * dot_code = (char *) calloc(size, sizeof(char));

    char * nodes = CreateNodes (list, size);
    char * vals  = CreateVals  (list, size);
    char * edges = CreateEdges (list, size);

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

char * CreateVals (const List * list, size_t size)
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

char * CreateNodes (const List * list, size_t size)
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

char * CreateEdges (const List * list, size_t size)
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
        sprintf(edges, "\tnode_%d -> node_%d[weight = 10, style = invis];\n %n", i, i + 1, &symbs);
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

char * CreateAddInfo(size_t err_vec)
{
    char * add_info = (char *) calloc(1000, sizeof(char));
    char * add_info_init = add_info;

    time_t t = time(NULL);

    tm * loc_time = localtime(&t);

    int symbs = 0;

    sprintf(add_info, "subgraph cluster_add_info_%d{\n"
                      "node_add_info [shape = plaintext, label = \" %s \"];\n"
                      "}\n%n", rand(), asctime(loc_time), &symbs);

    add_info += symbs;

    char * err_info = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));
    char * err_info_init = err_info;

    if (err_vec & LST_ERR_NO_LIST_PTR)
    {
        sprintf(err_info, "List nullptr\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_NO_DATA_PTR)
    {
        sprintf(err_info, "List data nullptr\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_NO_NEXT_PTR)
    {
        sprintf(err_info, "List next nullptr\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_NO_PREV_PTR)
    {
        sprintf(err_info, "List prev nullptr\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_HEAD_TAIL)
    {
        sprintf(err_info, "List head or tail incorrect\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_CHAIN)
    {
        sprintf(err_info, "List chain in next or prev has loops\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_FRE_PREV)
    {
        sprintf(err_info, "Free list node has prev != -1\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_FRE)
    {
        sprintf(err_info, "List fre incorrect\n%n", &symbs);
        err_info += symbs;
    }
    if (err_vec & LST_ERR_SIZE)
    {
        sprintf(err_info, "List size incorrect\n%n", &symbs);
        err_info += symbs;
    }

    if (err_info != err_info_init)
    {
        sprintf(add_info, "subgraph cluster_err_info{\n"
                      "node_err_info [shape = plaintext, label = \" %s \"];\n}\n", err_info_init);
    }

    free(err_info_init);

    return add_info_init;
}
