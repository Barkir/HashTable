
#include <stdlib.h>

#include "list.h"


int ListCtor(List * lst)
{
    lst = malloc(sizeof(List));
    if (!lst) return LST_MEMALLOC_ERROR;

    return LST_SUCCESS;
}

int ListInsert(List * lst, char * str)
{
    while (lst)
        lst = lst->nxt;

    lst = malloc(sizeof(List));
    lst->elem = str;

    return LST_SUCCESS;
}
