#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xmmintrin.h>
#include <x86intrin.h>
#include <nmmintrin.h>

#include "main.h"

int main(int argc, char * argv[])
{
    switch(ProcessCmd(argc, argv))
    {
        case IO_DEFAULT: return DefaultVersion();
        case IO_ALIGNED: return SIMDVersion();

    }

    return 0;
}

int SIMDVersion(void)
{

    Htable * tab = NULL;
    if (HtableInit(&tab, HTABLE_BINS))
        return ParseHtableError(HTABLE_INIT_ERROR);

    char ** harry = NULL;
    char ** bible = NULL;

    File2Lines(IO_DEFAULT, &harry, "info/parsed.harry");
    File2Lines(IO_DEFAULT, &bible, "info/parsed.bible");


    for (int i = 0; i < NUM_WORDS; i++)
    {
        // LOGGER("%s", harry[i]);
        HtableOptInsert(tab, harry[i]);
    }

    char * found = NULL;
    // for (int j = 0; j < 100; j++)
    // {
    for (int i = 0; i < NUM_WORDS * 300; i++)
    {
        if (HtableOptFind(tab, bible[i], found) == HTABLE_FOUND)
        {
            // LOGGER("FOUND WORD %s", bible[i]);
        }
        else
        {
            // LOGGER("NOT FOUND WORD %s", bible[i]);
        }
    }
    // }


    HtableDump(tab);
    HtableDestroy(tab);

    return 0;
}

int DefaultVersion(void)
{
    Htable * tab = NULL;
    if (HtableInit(&tab, HTABLE_BINS))
        return ParseHtableError(HTABLE_INIT_ERROR);


    char ** harry = NULL;
    char ** bible = NULL;

    File2Lines(IO_DEFAULT, &harry, "info/parsed.harry");
    File2Lines(IO_DEFAULT, &bible, "info/parsed.bible");

    for (int i = 0; i < NUM_WORDS; i++)
    {
        HtableInsert(tab, harry[i]);
    }

    char * found = NULL;
    for (int i = 0; i < NUM_WORDS * 300; i++)
    {
        if (HtableFind(tab, bible[i], found) == HTABLE_FOUND)
        {
            LOGGER("FOUND WORD %s", bible[i]);
        }
        else
        {
            LOGGER("NOT FOUND WORD %s", bible[i]);
        }
    }


    HtableDump(tab);

    HtableDestroy(tab);
    return 0;
}
