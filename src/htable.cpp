#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>
#include <x86intrin.h>
#include <nmmintrin.h>
#include <immintrin.h>
#include <wmmintrin.h>

#include "list.h"
#include "htable.h"
#include "errors.h"
#include "IO.h"
#include "crc32.h"

int HtableInit(Htable ** tab, size_t bins)
{
    *tab = (Htable*) calloc(BUF_LEN, sizeof(Htable));
    if (!tab) return ParseHtableError(HTABLE_MEMALLOC_ERROR);

    (*tab)->bins = bins;

    (*tab)->table = (List**) calloc(BUF_LEN, bins * sizeof(List*));
    if (!(*tab)->table) return ParseHtableError(HTABLE_MEMALLOC_ERROR);


    return HTABLE_SUCCESS;
}

int HtableDestroy(Htable * tab)
{
    for (int i = 0; i < tab->bins; i++)
    {
        List * lst = tab->table[i];
        while (lst)
        {
            List * e = lst;
            lst = lst->nxt;
            free(e->elem);
            free(e);
        }
    }

    free(tab->table);
    free(tab);
    return HTABLE_SUCCESS;
}

int HtableInsert(Htable * tab, const char * string)
{
    size_t ind = crc32_naive(string, strlen(string), CRC32INIT) % tab->bins;
    for (List * lst = tab->table[ind]; lst; lst=lst->nxt)
    {

    }

    List * n = (List*) calloc(1, sizeof(List));
    if (!n) return HTABLE_MEMALLOC_ERROR;

    n->elem = (char*) calloc(BUF_LEN, BUF_LEN);
    if (!n->elem) return HTABLE_MEMALLOC_ERROR;

    memcpy(n->elem, string, strlen(string) + 1);

    n->nxt = tab->table[ind];
    tab->table[ind] = n;


    return HTABLE_SUCCESS;
}

int HtableOptInsert(Htable * tab, const char * string)
{
    if (!tab) return HTABLE_NULLPTR;
    if (!string) return HTABLE_NULLPTR;

    int bin = crc32_intinsic(string) % tab->bins;
    for (List * lst = tab->table[bin]; lst; lst=lst->nxt)
    {

    }

    List * n = (List*) calloc(1, sizeof(List));
    if (!n) return HTABLE_MEMALLOC_ERROR;

    n->elem = (char*) calloc(BUF_LEN, 1);
    if (!n->elem) return HTABLE_MEMALLOC_ERROR;

    memcpy(n->elem, string, strlen(string) + 1);

    n->nxt = tab->table[bin];
    tab->table[bin] = n;

    return HTABLE_SUCCESS;
}

int HtableFind(Htable * tab, const char * string, char * result)
{
    int bin = crc32_naive(string, strlen(string), CRC32INIT) % tab->bins;

    for (List * lst = tab->table[bin]; lst; lst = lst->nxt)
    {
        if (!strcmp(string, lst->elem)) return HTABLE_FOUND;
    }

    return HTABLE_NOT_FOUND;
}

// int HtableOptFind(Htable * tab, const char * string, char * result)
// {
//     int bin = crc32_intinsic(string) % tab->bins;
//     for (List * lst = tab->table[bin]; lst; lst = lst->nxt)
//     {
//         // LOGGER("%ld, %ld", (size_t) string % 32, (size_t) lst->elem % 32);
//         if (strcmp_asm(string, lst->elem))
//         {
//             // LOGGER("FOUND WORD %s, bin = %d", string, bin);
//             return HTABLE_FOUND;
//         }
//     }
//
//     // LOGGER("NOT FOUND WORD %s, bin = %d", string, bin);
//
//     return HTABLE_NOT_FOUND;
// }

int HtableDump(Htable * tab)
{
    FILE * file = fopen("info/log.dmp", "w+");
    if (!file) return  HTABLE_FILE_OPEN_ERROR;
    fprintf(file, "<<<LOG FILE OF HASH TABLE>>>\n");

    fprintf(file, "Load factor = %f\n", HashLoadFactor(tab));

    for (int bins = 0; bins < tab->bins; bins++)
    {
        fprintf(file, "[BIN %d]", bins);
        fprintf(file, "----------------------------------\n");
        for (List * lst = tab->table[bins]; lst; lst=lst->nxt)
        if (lst->elem) fprintf(file, "\t %s", lst->elem);
        fprintf(file, "----------------------------------\n");
    }

    return HTABLE_SUCCESS;
}

float HashLoadFactor(Htable * tab)
{
    float lst_count = 0;
    for (int i = 0; i < tab->bins; i++)
    {
        for (List * lst = tab->table[i]; lst; lst = lst->nxt, lst_count++)
        {}
    }

    return lst_count / tab->bins;
}
