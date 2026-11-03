#include <stdio.h>
#include "log.h"
#include "linked_list.h"
#include "graphviz.h"

int main(void)
{
    restart_log();

    LinkedListError error = LL_OK;
    LinkedList list = {};
    NodeID node_id = {};
    list_el_t value = 0;

    error = list_init(&list, 4);
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
    error = list_insert_head(&list, 10, &node_id);

    error = list_insert_after(&list, node_id, 40, &node_id);
    error = list_insert_before(&list, node_id, 22120, &node_id);
    error = list_insert_after(&list, node_id, 222, &node_id);

    // list.data[1].next = 3;

    error = list_remove(&list, (NodeID){.index = 4});
    error = list_remove(&list, (NodeID){.index = list.head});

    error = list_destroy(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", list_get_str_error(error));
        return error;
    }

    close_log();

    return 0;
}
