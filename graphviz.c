#include "graphviz.h"
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"
#include "config.h"
#include "log.h"

#ifndef NLLDEBUG

static int generate_graph_file(LinkedList *list);

// Возвращает блок динамической памяти с названием файла, в который записано изображение с графом
char *draw_graph(LinkedList *list)
{
    assert(list);

    if (list->data == NULL)
        return NULL;

    if (generate_graph_file(list))
    {
        lllog("Error: can not generate .dot file with graph((");
        return NULL;
    }

    struct timespec ts = {};
    timespec_get(&ts, TIME_UTC);
    struct tm *local_time = localtime(&ts.tv_sec);

    char *filename = (char *)calloc(MAX_FILENAME_LENGTH, sizeof(char));
    if (filename == NULL)
    {
        lllog("Error: can not allocate memory for graph filename");
        return NULL;
    }

    if (snprintf(filename, MAX_FILENAME_LENGTH, "%s/%04d.%02d.%02d_%02d:%02d:%02d:%03ld.png", GRAPH_DIRECTORY,
                 local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
                 local_time->tm_hour, local_time->tm_min, local_time->tm_sec, ts.tv_nsec) < 0)
    {
        lllog("Error: can not write graph filename to buf");
        free_ptr(filename);
        return NULL;
    }

    // с учетом 2 по \0 в константах можно не делать +2, но так надежнее)
    char graph_dir_path[sizeof(LOG_DIRECTORY) + sizeof(GRAPH_DIRECTORY) + 2] = {};
    if (snprintf(graph_dir_path, sizeof(graph_dir_path), "%s/%s", LOG_DIRECTORY, GRAPH_DIRECTORY) < 0)
    {
        lllog("Error: can not write graph_dir_path to buffer");
        free_ptr(filename);
        return NULL;
    }

    create_folder_if_not_exists(graph_dir_path);

    char command[MAX_SYS_COMMAND_LENGTH] = {};
    if (snprintf(command, sizeof(command), "dot %s -Tpng -o %s/%s", DOT_FILE_NAME, LOG_DIRECTORY, filename) < 0)
    {
        lllog("Error: can not write sys command to buf");
        free_ptr(filename);
        return NULL;
    }

    system(command);

    return filename;
}

int generate_graph_file(LinkedList *list)
{
    assert(list);

    if (list->data == NULL)
        return 1;

    FILE *file = fopen(DOT_FILE_NAME, "w");
    if (file == NULL)
    {
        lllog("Can not open file %s", DOT_FILE_NAME);
        return 1;
    }

    fprintf(file, "digraph {\n");
    fprintf(file, "  rankdir = LR;\n  splines=ortho;\n  nodesep=0.6;\n  ranksep=1.0;\n  node [shape = Mrecord; style = filled;];\n  edge [color = \"%s\";];\n\n", EDGE_COLOR);

    // Обьявляем все узлы с доп. инфо пометками
    for (size_t i = 0; i < list->capacity; i++)
    {
        struct Node *node = &list->data[i];
        const char *hint = "\0";
        const char *hint_label = "\0";
        const char *fill_color = NULL, *color = NULL;

        NodeError error = node_assert(list, (off_t)i);
        if (error == NODE_FREE)
        {
            hint = i == list->free ? "FREE" : "\0"; // если это первый свободный элемент то делаем пометку
            hint_label = hint;
            fill_color = FREE_FILL_COLOR;
            color = FREE_COLOR;
        }
        else if (error != NODE_OK)
        {
            hint = "\0";
            hint_label = hint;
            fill_color = ERROR_FILL_COLOR;
            color = ERROR_COLOR;
        }
        else if (i == 0)
        {
            hint = "ZERO";
            hint_label = hint;
            fill_color = ZERO_FILL_COLOR;
            color = ZERO_COLOR;
        }
        else if (node->prev == 0 && node->next == 0) // единственный элемент
        {
            hint = "HEADTAIL";
            hint_label = "HEAD\\n&\\nTAIL";
            fill_color = HEAD_FILL_COLOR;
            color = HEAD_COLOR;
        }
        else if (i == (size_t)list->head) // голова
        {
            hint = "HEAD";
            hint_label = hint;
            fill_color = HEAD_FILL_COLOR;
            color = HEAD_COLOR;
        }
        else if (i == (size_t)list->tail) // хвост
        {
            hint = "TAIL";
            hint_label = hint;
            fill_color = TAIL_FILL_COLOR;
            color = TAIL_COLOR;
        }
        else
        {
            hint = "\0";
            hint_label = hint;
            fill_color = NODE_FILL_COLOR;
            color = NODE_COLOR;
        }

        if (hint[0])
        {
            fprintf(file, "  %s [label = \"%s\"; fillcolor=\"%s\"; color=\"%s\"];\n  { rank = same; %s -> V%zu [color=\"%s\";]}\n",
                    hint, hint_label, fill_color, color, hint, i, color);
        }

        fprintf(file, "  V%zu [label = \"index: %zu | value: " LIST_EL_SPECIFICATOR " | {prev: %lld | next: %lld}\"; fillcolor = \"%s\"; color = \"%s\";];\n",
                i, i, node->value, node->prev, node->next, fill_color, color);
    }
    fprintf(file, "\n");

    // связываем все элементы подряд невидимыми ребрами (но только если в списке вообще есть элементы)
    for (size_t i = 0; list->capacity > 0 && i < list->capacity - 1; i++)
    {

        fprintf(file, "  V%zu -> V%zu [style=invis];\n", i, i + 1);
    }

    // соединяем элементы уже видимыми стрелками
    for (size_t i = 1; i < list->capacity; i++)
    {
        struct Node *node = &list->data[i];

        assert(node->next < (off_t)list->capacity);

        NodeError error = node_assert(list, (off_t)i);
        if (error == NODE_FREE)
        {
            if (node->next != 0)
                fprintf(file, "  V%zu -> V%lld [constraint=false; color=\"%s\";];\n", i, node->next, FREE_EDGE_COLOR);
        }
        else if (error == NODE_OK)
        {
            if ((off_t)i != list->tail)
                fprintf(file, "  V%zu -> V%lld [dir=both; constraint=false];\n", i, node->next);
        }
        else // ошибка
        {
            if (node->next >= 0) // показываем куда ведет ошибочная стрелка только если элемент назначения есть
                fprintf(file, "  V%zu -> V%lld [constraint=false; color=\"%s\";];\n", i, node->next, ERROR_EDGE_COLOR);
        }
    }

    fprintf(file, "}\n");
    fclose(file);

    return 0;
}

#else

char *draw_graph(LinkedList *list)
{
    return NULL;
}

#endif // NLLDEBUG
