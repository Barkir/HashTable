#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xmmintrin.h>
#include <x86intrin.h>
#include <nmmintrin.h>

#include "list.h"
#include "htable.h"
#include "errors.h"
#include "IO.h"

int SIMDVersion(void);
int DefaultVersion(void);
const int ALIGNMENT = 16;

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
    if (HtableInit(&tab, HTABLE_BINS, HashFunction, ListInsertStud))
        return ParseHtableError(HTABLE_INIT_ERROR);


    char ** harry = NULL;
    char ** bible = NULL;

    File2Lines(IO_ALIGNED, &harry, "info/parsed16.harry");
    File2Lines(IO_ALIGNED, &bible, "info/parsed16.bible");

    for (int i = 0; i < NUM_WORDS; i++)
    {
        // LOGGER("%s", harry[i]);
        HtableAlignedInsert(ALIGNMENT, tab, harry[i]);
    }

    char * found = NULL;
    for (int i = 0; i < NUM_WORDS * 300; i++)
    {
        if (HtableAlignedFind(ALIGNMENT, tab, bible[i], found) == HTABLE_FOUND)
        {
            // LOGGER("FOUND WORD %s", bible[i]);
        }
        else
        {
            // LOGGER("NOT FOUND WORD %s", bible[i]);
        }
    }


    // HtableDump(tab);
    // HtableDestroy(tab);
    return 0;
}

int DefaultVersion(void)
{
    Htable * tab = NULL;
    if (HtableInit(&tab, HTABLE_BINS, HashFunction, ListInsertStud))
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
            // LOGGER("FOUND WORD %s", bible[i]);
        }
        else
        {
            // LOGGER("NOT FOUND WORD %s", bible[i]);
        }
    }


    HtableDump(tab);

    HtableDestroy(tab);
    return 0;
}
