#include <stdio.h>
#include "log.h"
#include "linked_list.h"

int main(void)
{
    restart_log();

    LinkedListError error = LL_OK;
    LinkedList list = {};
    NodeID node_id = {};

    error = list_init(&list, 100);
    if (error != LL_OK)
    {
        LOG("Error while initialization linked list: %s", list_get_str_error(error));
        return error;
    }

    for (int i = 1; i < 1000; i++)
    {
        error = list_insert_start(&list, i * 10, &node_id);
        if (error != LL_OK)
        {
            LOG("Error while initialization linked list: %s", list_get_str_error(error));
            return error;
        }
        list_dump(&list);
    }

    error = list_insert_after(&list, node_id, 35, &node_id);

    list_dump(&list);

    error = list_insert_before(&list, node_id, 37, &node_id);

    list_dump(&list);

    error = list_destroy(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", list_get_str_error(error));
        return error;
    }

    return 0;
}
