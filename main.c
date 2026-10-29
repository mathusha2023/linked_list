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

    list_insert_end(&list, -20, &node_id);
    list_insert_start(&list, -10, &node_id);

    list_dump(&list);

    for (int i = 1; i < 20; i++)
    {
        error = list_insert_after(&list, node_id, i * 10, &node_id);
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

    error = list_get_value(&list, node_id, &value);
    printf("Value1: " LIST_EL_SPECIFICATOR "\n", value);

    error = list_set_value(&list, (NodeID){.index = node_id.index - 1}, 67);

    list_dump(&list);

    error = list_get_value(&list, (NodeID){.index = node_id.index - 1}, &value);
    printf("Value2: " LIST_EL_SPECIFICATOR "\n", value);

    error = list_insert_after(&list, (NodeID){.index = list.head}, 1488, NULL);

    list_dump(&list);

    error = list_destroy(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", list_get_str_error(error));
        return error;
    }

    return 0;
}
