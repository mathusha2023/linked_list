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
#define LIST_EL_SPECIFICATOR "%d"

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
    size_t capacity;
    // size_t free;
} LinkedList;

// API для взаимодействия с двусвязным списком
LinkedListError list_init(LinkedList *list, size_t start_size);
LinkedListError list_destroy(LinkedList *list);
LinkedListError list_assert(LinkedList *list);
const char *list_get_str_error(LinkedListError error);

#endif // LINKED_LIST_H
