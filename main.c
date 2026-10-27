#include <stdio.h>
#include "log.h"
#include "linked_list.h"

int main(void)
{
    LOG("Welcome to linked list world!");

    LinkedListError error = LL_OK;
    LinkedList list = {};

    error = init_list(&list);
    if (error != LL_OK)
    {
        LOG("Error while initialization linked list: %s", get_str_ll_error(error));
        return error;
    }

    error = destroy_list(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", get_str_ll_error(error));
        return error;
    }

    LOG("Goodbye!");

    return 0;
}
