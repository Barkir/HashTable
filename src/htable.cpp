#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "htable.h"
#include "errors.h"

int HashTableInit(Htable ** tab, size_t bins, HashFunc hfunc, InsertFunc ifunc)
{
    *tab = (Htable*) calloc(1, sizeof(Htable));
    if (!tab) return ParseHtableError(HTABLE_MEMALLOC_ERROR);

    (*tab)->bins = bins;
    (*tab)->hfunc = hfunc;
    (*tab)->ifunc = ifunc;


    (*tab)->table = (List**) calloc(bins, sizeof(List*));
    if (!(*tab)->table) return ParseHtableError(HTABLE_MEMALLOC_ERROR);


    return HTABLE_SUCCESS;
}

int HashTableInsert(Htable * tab, const char * string)
{


    size_t ind = tab->hfunc(string, strlen(string)) % tab->bins;

    for (List * lst = tab->table[ind]; lst; lst=lst->nxt)
    {

    }

    List * n = (List*) malloc(sizeof(List));
    n->elem = strdup(string);
    n->nxt = tab->table[ind];
    tab->table[ind] = n;


    return HTABLE_SUCCESS;
}

int64_t ListInsertStud(void * lst, const void * elem)
{
    if (!ListInsert((List*) lst, (const char*) elem))
        return ParseListError(LST_INSERT_ERROR);

    return LST_SUCCESS;
}

int64_t HashFunction(const void * elem, size_t size)
{
    int64_t hash = 0x3de5788a2dae7e9;
    const char * str = (const char *) elem;
    for (int i = 0; i < size; i++)
    {
        hash = (hash >> 25) | (hash << 39);
        hash = (str[i] * 0xa5a5013fe20a5a83) ^ hash;
        hash = (hash >> 39) | (hash << 25);
    }

    return hash;
}

int HtableDump(Htable * tab)
{
    FILE * file = fopen("log.dmp", "w+");
    if (!file) return  HTABLE_FILE_OPEN_ERROR;
    fprintf(file, "<<<LOG FILE OF HASH TABLE>>>\n");

    fprintf(file, "Load factor = %f\n", HashLoadFactor(tab));

    for (int bins = 0; bins < tab->bins; bins++)
    {
        fprintf(file, "[BIN %d]", bins);
        fprintf(file, "----------------------------------\n");
        for (List * lst = tab->table[bins]; lst; lst=lst->nxt)
            fprintf(file, "\t %s", lst->elem);
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
