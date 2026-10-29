#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "config.h"
#include "log.h"

static LinkedListError insert_node(LinkedList *list, struct Node *node, off_t *index);

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

LinkedListError list_get_value(LinkedList *list, NodeID node_id, list_el_t *value)
{
    assert(list);
    assert(value);

    lllog("Starting list [%p] getting element at index %llu", list, node_id.index);

    struct Node node = list->data[node_id.index];
    *value = node.value;

    lllog("List [%p] getting element at index %llu successful: " LIST_EL_SPECIFICATOR,
          list, node_id.index, *value);

    return LL_OK;
}

LinkedListError list_set_value(LinkedList *list, NodeID node_id, list_el_t value)
{
    assert(list);

    lllog("Starting list [%p] setting value " LIST_EL_SPECIFICATOR " to element at index %llu",
          list, value, node_id.index);

    struct Node *node = &list->data[node_id.index];
    node->value = value;

    lllog("List [%p] setting value " LIST_EL_SPECIFICATOR " to element at index %llu successful",
          list, value, node_id.index);

    return LL_OK;
}

LinkedListError list_get_head(LinkedList *list, NodeID *head_node_id)
{
    assert(list);
    assert(head_node_id);

    lllog("Starting list [%p] getting head", list);

    head_node_id->index = list->head;

    lllog("List [%p] getting head successful", list);

    return LL_OK;
}

LinkedListError list_get_tail(LinkedList *list, NodeID *tail_node_id)
{
    assert(list);
    assert(tail_node_id);

    lllog("Starting list [%p] getting tail", list);

    tail_node_id->index = list->tail;

    lllog("List [%p] getting tail successful", list);

    return LL_OK;
}

LinkedListError list_get_next(LinkedList *list, NodeID node_id, NodeID *next_node_id)
{
    assert(list);
    assert(next_node_id);

    lllog("Starting list [%p] getting next element at index %llu", list, node_id.index);

    struct Node node = list->data[node_id.index];
    off_t next = node.next;
    if (next == -1)
    {
        lllog("Next element next field is -1, error");
        return LL_GETTING_VOID_NODE;
    }
    if (next == 0)
    {
        return LL_LAST_ELEMENT;
    }

    next_node_id->index = next;

    lllog("List [%p] getting next element at index %llu successful: new id %lld", list, node_id.index, next);

    return LL_OK;
}

LinkedListError list_get_prev(LinkedList *list, NodeID node_id, NodeID *prev_node_id)
{
    assert(list);
    assert(prev_node_id);

    lllog("Starting list [%p] getting prev element at index %llu", list, node_id.index);

    struct Node node = list->data[node_id.index];
    off_t prev = node.prev;
    if (prev == -1)
    {
        lllog("Next element next field is -1, error");
        return LL_GETTING_VOID_NODE;
    }
    if (prev == 0)
    {
        return LL_FIRST_ELEMENT;
    }

    prev_node_id->index = prev;

    lllog("List [%p] getting prev element at index %llu successful: new id %lld", list, node_id.index, prev);

    return LL_OK;
}

// если new_head_node_id != NULL, то в него записывается node_id нового элемента
LinkedListError list_insert_end(LinkedList *list, list_el_t value, NodeID *new_head_node_id)
{
    assert(list);

    lllog("Starting list [%p] inserting value " LIST_EL_SPECIFICATOR " to head", list, value);

    struct Node *old_head = &list->data[list->head];
    struct Node new_head = {
        .value = value,
        .prev = list->head,
        .next = 0};

    off_t new_head_ind = 0;
    LinkedListError error = insert_node(list, &new_head, &new_head_ind);
    if (error != LL_OK)
    {
        lllog("Error while inserting node to list: %s", list_get_str_error(error));
        return error;
    }

    if (list->head != 0)
    {
        old_head->next = new_head_ind;
    }

    list->head = new_head_ind;

    if (new_head_node_id != NULL)
    {
        new_head_node_id->index = new_head_ind;
    }

    if (list->tail == 0)
    {
        list->tail = new_head_ind;
    }

    lllog("List [%p] inserting value " LIST_EL_SPECIFICATOR " to head successful: at index %lld",
          list, value, new_head_ind);

    return LL_OK;
}

// если new_tail_node_id != NULL, то в него записывается node_id нового элемента
LinkedListError list_insert_start(LinkedList *list, list_el_t value, NodeID *new_tail_node_id)
{
    assert(list);

    lllog("Starting list [%p] inserting value " LIST_EL_SPECIFICATOR " to tail", list, value);

    struct Node *old_tail = &list->data[list->tail];
    struct Node new_tail = {
        .value = value,
        .prev = 0,
        .next = list->tail};

    off_t new_tail_ind = 0;
    LinkedListError error = insert_node(list, &new_tail, &new_tail_ind);
    if (error != LL_OK)
    {
        lllog("Error while inserting node to list: %s", list_get_str_error(error));
        return error;
    }

    if (list->tail != 0)
    {
        old_tail->prev = new_tail_ind;
    }

    list->tail = new_tail_ind;

    if (new_tail_node_id != NULL)
    {
        new_tail_node_id->index = new_tail_ind;
    }

    if (list->head == 0)
    {
        list->head = new_tail_ind;
    }

    lllog("List [%p] inserting value " LIST_EL_SPECIFICATOR " to tail successful: at index %lld",
          list, value, new_tail_ind);

    return LL_OK;
}

LinkedListError list_insert_after(LinkedList *list, NodeID node_id,
                                  list_el_t value, NodeID *new_el_node_id)
{
    assert(list);

    lllog("Starting list [%p] inserting value " LIST_EL_SPECIFICATOR " after %lld index", list, value, node_id.index);

    struct Node *prev_node = &list->data[node_id.index];
    if (prev_node->next == 0)
    {
        lllog("Insert_after will be replaced with insert_head");
        return list_insert_end(list, value, new_el_node_id);
    }

    struct Node *next_node = &list->data[prev_node->next];
    struct Node new_el = {
        .value = value,
        .prev = node_id.index,
        .next = prev_node->next,
    };

    off_t new_el_index = 0;
    LinkedListError error = insert_node(list, &new_el, &new_el_index);
    if (error != LL_OK)
    {
        lllog("Error while inserting node to list: %s", list_get_str_error(error));
        return error;
    }

    prev_node->next = new_el_index;
    next_node->prev = new_el_index;

    if (new_el_node_id != NULL)
    {
        new_el_node_id->index = new_el_index;
    }

    lllog("List [%p] inserting value " LIST_EL_SPECIFICATOR " after %lld index successful: new index - %lld", list, value, node_id.index, new_el_index);

    return LL_OK;
}

LinkedListError list_insert_before(LinkedList *list, NodeID node_id,
                                   list_el_t value, NodeID *new_el_node_id)
{
    assert(list);

    lllog("Starting list [%p] inserting value " LIST_EL_SPECIFICATOR " before %lld index", list, value, node_id.index);

    struct Node *next_node = &list->data[node_id.index];
    if (next_node->prev == 0)
    {
        lllog("Node to insert before is tail, replaced with list_insert_start");
        return list_insert_start(list, value, new_el_node_id);
    }

    lllog("Inserting before node %lld replaced with inserting node after %lld", node_id.index, next_node->prev);
    return list_insert_after(list, (NodeID){.index = next_node->prev}, value, new_el_node_id);
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
        lllog("Error: list capacity is null");
        return LL_NOT_INITIALIZED;
    }
    return LL_OK;
}

void list_dump(LinkedList *list)
{
    assert(list);

    lllog("List [%p] dump ");
    lllog("{");
    lllog("    head = %lld", list->head);
    lllog("    tail = %lld", list->tail);
    lllog("    capacity = %llu", list->capacity);
    lllog("");
    lllog("    #   value   next   prev   note");

    size_t i = 0;
    const size_t note_length = 15;
    char note[note_length] = {};

    while (i < list->capacity)
    {
        struct Node node = list->data[i];

        if (i == 0)
        {
            strncpy(note, "ZERO", note_length);
        }
        else if (node.prev == 0 && node.next == 0)
        {
            strncpy(note, "TAIL & HEAD", note_length);
        }
        else if (node.prev == 0)
        {
            strncpy(note, "TAIL", note_length);
        }
        else if (node.next == 0)
        {
            strncpy(note, "HEAD", note_length);
        }
        else if (node.prev == -1)
        {
            strncpy(note, "POIZON", note_length);
        }
        else
        {
            note[0] = '\0';
        }

        lllog("%5llu " LIST_EL_SPECIFICATOR " %6lld %6lld   %s", i, node.value, node.next, node.prev, note);

        i++;
    }
    lllog("");
    lllog("}");
}

const char *list_get_str_error(LinkedListError error)
{
    return LINKED_LIST_STR_ERRORS[error];
}

// просто засовывает узел на первую свободную позицию и отдает в off_t *index указатель на эту позицию
static LinkedListError insert_node(LinkedList *list, struct Node *node, off_t *index)
{
    assert(list);
    assert(node);

    size_t i = 1;
    while (list->data[i].prev >= 0 && i < list->capacity)
        i++;

    if (i == list->capacity)
    {
        lllog("Error: data array oferflowed!");
        return LL_OVERFLOW;
    }

    list->data[i] = *node;
    *index = (off_t)i;
    return LL_OK;
}
