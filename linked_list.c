#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>
#include "config.h"
#include "log.h"

LinkedListError list_init(LinkedList *list, size_t start_size)
{
    assert(list);
    lllog("Starting list [%p] initialization...", list);

    if (list->data != NULL)
    {
        lllog("Initialization error: list is not empty");
        return LL_ALREADY_INITIALIZED;
    }

    struct Node *data = (struct Node *)calloc(start_size + 1, sizeof(struct Node));
    if (data == NULL)
    {
        lllog("Error while allocating memory for list");
        return LL_MEMORY_ALLOCATION_ERROR;
    }

    list->data = data;
    list->head = 0;
    list->tail = 0;
    list->capacity = start_size + 1;

    struct Node void_node = {.value = 0, .prev = -1, .next = -1};

    for (size_t i = 1; i < list->capacity; i++)
    {
        assert(i < list->capacity);
        data[i] = void_node;
    }

    lllog("List [%p] initialization successful!", list);

    return LL_OK;
}

LinkedListError list_destroy(LinkedList *list)
{
    assert(list);

    lllog("Starting list [%p] destroing...", list);

    LinkedListError error = list_assert(list);
    if (error != LL_OK)
    {
        lllog("Error while destroing list: %s", list_get_str_error(error));
        return error;
    }

    free_ptr(list->data);
    list->head = 0;
    list->tail = 0;
    list->capacity = 0;

    lllog("List [%p] destroing successful!", list);

    return LL_OK;
}

LinkedListError list_assert(LinkedList *list)
{
    assert(list);

    if (list->data == NULL)
    {
        lllog("Error: list data is null");
        return LL_NOT_INITIALIZED;
    }
    if (!list->capacity)
    {
        lllog("Error: list length is null");
        return LL_NOT_INITIALIZED;
    }
    return LL_OK;
}

const char *list_get_str_error(LinkedListError error)
{
    return LINKED_LIST_STR_ERRORS[error];
}
