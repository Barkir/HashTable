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


    char ** lines = NULL;
    File2Lines(&lines, "parsed.lib");

    for (int i = 0; i < NUM_WORDS; i++)
        HtableInsert(tab, lines[i]);

    char * found = NULL;
    for (int j = 0; j < 100; j++)
    {
        for (int i = 0; i < NUM_WORDS; i++)
        {
            if (HtableFind(tab, lines[i], found) == HTABLE_FOUND)
            {
                LOGGER("FOUND WORD %s", lines[i]);
            }
            else
            {
                LOGGER("NOT FOUND WORD %s", lines[i]);
            }

        }
    }

    HtableDump(tab);

    HtableDestroy(tab);
    return 0;
}
