#include <stdio.h>
#include "log.h"
#include "linked_list.h"

int main(void)
{
    restart_log();

    LinkedListError error = LL_OK;
    LinkedList list = {};
    NodeID node_id = {};
    list_el_t value = 0;

    error = list_init(&list, 1);
    if (error != LL_OK)
    {
        LOG("Error while initialization linked list: %s", list_get_str_error(error));
        return error;
    }

    error = list_insert_head(&list, 10, &node_id);
    if (error != LL_OK)
    {
        LOG("Error while inserting el to head: %s", list_get_str_error(error));
        return error;
    }

    error = list_insert_after(&list, node_id, 20, &node_id);
    error = list_insert_tail(&list, 10, &node_id);

    error = list_remove(&list, node_id);
    if (error != LL_OK)
    {
        LOG("Error while removing el: %s", list_get_str_error(error));
        return error;
    }

    error = list_destroy(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", list_get_str_error(error));
        return error;
    }

    return 0;
}
