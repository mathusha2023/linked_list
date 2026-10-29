#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <sys/types.h>

// расскомментировать для отключения специальных дебагерских логов
// #define NLLDEBUG

#ifndef NLLDEBUG
#define lllog(message, ...) FLOG(message, ##__VA_ARGS__)
#else
#define lllog(message, ...)
#endif // NLLDEBUG

// настройки типа списка
typedef int list_el_t;
#define LIST_EL_SPECIFICATOR "%7d"

typedef enum LinkedListError
{
    LL_OK,
    LL_NOT_INITIALIZED,
    LL_ALREADY_INITIALIZED,
    LL_MEMORY_ALLOCATION_ERROR,
    LL_GETTING_VOID_NODE,
    LL_LAST_ELEMENT,
    LL_FIRST_ELEMENT,
    LL_OVERFLOW,
} LinkedListError;

static const char *LINKED_LIST_STR_ERRORS[] = {
    "LL_OK",
    "LL_NOT_INITIALIZED",
    "LL_ALREADY_INITIALIZED",
    "LL_MEMORY_ALLOCATION_ERROR",
    "LL_GETTING_VOID_NODE",
    "LL_LAST_ELEMENT",
    "LL_FIRST_ELEMENT",
    "LL_OVERFLOW",
};

struct Node
{
    list_el_t value;
    off_t prev;
    off_t next;
};

typedef struct NodeID
{
    off_t index;
} NodeID;

typedef struct LinkedList
{
    struct Node *data;
    off_t head;
    off_t tail;
    size_t capacity;
    // size_t free;
} LinkedList;

// API для взаимодействия с двусвязным списком
LinkedListError list_init(LinkedList *list, size_t start_size);
LinkedListError list_destroy(LinkedList *list);

LinkedListError list_get_value(LinkedList *list, NodeID node_id, list_el_t *value);
LinkedListError list_set_value(LinkedList *list, NodeID node_id, list_el_t value);

LinkedListError list_get_head(LinkedList *list, NodeID *head_node_id);
LinkedListError list_get_tail(LinkedList *list, NodeID *tail_node_id);
LinkedListError list_get_next(LinkedList *list, NodeID node_id, NodeID *next_node_id);
LinkedListError list_get_prev(LinkedList *list, NodeID node_id, NodeID *prev_node_id);

LinkedListError list_insert_start(LinkedList *list, list_el_t value, NodeID *new_tail_node_id);
LinkedListError list_insert_end(LinkedList *list, list_el_t value, NodeID *new_head_node_id);
LinkedListError list_insert_after(LinkedList *list, NodeID node_id,
                                  list_el_t value, NodeID *new_el_node_id);
LinkedListError list_insert_before(LinkedList *list, NodeID node_id,
                                   list_el_t value, NodeID *new_el_node_id);

LinkedListError list_assert(LinkedList *list);

void list_dump(LinkedList *list);
const char *list_get_str_error(LinkedListError error);

#endif // LINKED_LIST_H
