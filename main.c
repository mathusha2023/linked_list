#include <stdio.h>
#include "log.h"
#include "linked_list.h"

int main(void)
{
    LOG("Welcome to linked list world!");

    LinkedListError error = LL_OK;
    LinkedList list = {};

    error = list_init(&list, 100);
    if (error != LL_OK)
    {
        LOG("Error while initialization linked list: %s", list_get_str_error(error));
        return error;
    }

    error = list_destroy(&list);
    if (error != LL_OK)
    {
        LOG("Error while destroing linked list: %s", list_get_str_error(error));
        return error;
    }

    LOG("Goodbye!");

    return 0;
}
