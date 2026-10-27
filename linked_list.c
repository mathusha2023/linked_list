#include "linked_list.h"
#include <stdlib.h>
#include "config.h"
#include "log.h"

LinkedListError init_list(LinkedList *list)
{
    assert(list);
    FLOG("Starting list %p initialization...", list);

    if (list->data != NULL)
    {
        LOG("Initialization error: list is not empty");
        return LL_ALREADY_INITIALIZED;
    }

    struct Node *data = (struct Node *)calloc(MAX_SIZE, sizeof(struct Node));
    if (data == NULL)
    {
        LOG("Error while allocating memory for list");
        return LL_MEMORY_ALLOCATION_ERROR;
    }

    list->data = data;
    list->head = 0;
    list->tail = 0;
    list->length = MAX_SIZE;

    struct Node void_node = {.value = 0, .prev = -1, .next = -1};

    for (size_t i = 1; i < MAX_SIZE; i++)
    {
        assert(i < list->length);
        data[i] = void_node;
    }

    FLOG("List %p initialization successful!", list);

    return LL_OK;
}

LinkedListError destroy_list(LinkedList *list)
{
    assert(list);

    FLOG("Starting list %p destroing...", list);

    LinkedListError error = assert_list(list);
    if (error != LL_OK)
    {
        LOG("Error while destroing list: %s", get_str_ll_error(error));
        return error;
    }

    free_ptr(list->data);
    list->head = 0;
    list->tail = 0;
    list->length = 0;

    FLOG("List %p destroing successful!", list);

    return LL_OK;
}

LinkedListError assert_list(LinkedList *list)
{
    assert(list);

    if (list->data == NULL)
    {
        LOG("Error: list data is null");
        return LL_NOT_INITIALIZED;
    }
    if (!list->length)
    {
        LOG("Error: list length is null");
        return LL_NOT_INITIALIZED;
    }
    return LL_OK;
}

const char *get_str_ll_error(LinkedListError error)
{
    return LINKED_LIST_STR_ERRORS[error];
}
