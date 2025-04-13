#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "htable.h"
#include "errors.h"

int main(void)
{
    Htable * tab = NULL;
    if (HashTableInit(&tab, HTABLE_BINS, HashFunction, ListInsertStud))
        return ParseHtableError(HTABLE_INIT_ERROR);

    for (int i = 0; i < 1000; i++)
        HashTableInsert(tab, "hellowrld");

    HtableDump(tab);

    return 0;
}
