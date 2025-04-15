#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "htable.h"
#include "errors.h"
#include "IO.h"

int File2Lines(char *** buf, const char * filename);

int main(void)
{
    Htable * tab = NULL;
    if (HtableInit(&tab, HTABLE_BINS, HashFunction, ListInsertStud))
        return ParseHtableError(HTABLE_INIT_ERROR);


    char ** harry = NULL;
    char ** bible = NULL;

    File2Lines(&harry, "info/parsed.lib");
    File2Lines(&bible, "info/parsed.bible");

    for (int i = 0; i < NUM_WORDS; i++)
    {
        HtableInsert(tab, harry[i]);
    }

    char * found = NULL;
    for (int j = 0; j < 1000; j++)
    {
        for (int i = 0; i < NUM_WORDS * 10; i++)
        {
            if (HtableFind(tab, bible[i], found) == HTABLE_FOUND)
            {
                // LOGGER("FOUND WORD %s", bible[i]);
            }
            else
            {
                // LOGGER("NOT FOUND WORD %s", bible[i]);
            }

        }
    }

    HtableDump(tab);

    HtableDestroy(tab);
    return 0;
}
