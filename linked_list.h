#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <sys/types.h>

typedef int list_el_t;

typedef enum LinkedListError
{
    LL_OK,
    LL_NOT_INITIALIZED,
    LL_ALREADY_INITIALIZED,
    LL_MEMORY_ALLOCATION_ERROR,
} LinkedListError;

static const char *LINKED_LIST_STR_ERRORS[] = {
    "LL_OK",
    "LL_NOT_INITIALIZED",
    "LL_ALREADY_INITIALIZED",
    "LL_MEMORY_ALLOCATION_ERROR",
};

struct Node
{
    list_el_t value;
    off_t prev;
    off_t next;
};

typedef struct LinkedList
{
    struct Node *data;
    size_t head;
    size_t tail;
    size_t length;
    // size_t free;
} LinkedList;

// API для взаимодействия с двусвязным списком
LinkedListError init_list(LinkedList *list);
LinkedListError destroy_list(LinkedList *list);
LinkedListError assert_list(LinkedList *list);
const char *get_str_ll_error(LinkedListError error);

#endif
