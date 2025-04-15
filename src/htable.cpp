#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "htable.h"
#include "errors.h"

int HtableInit(Htable ** tab, size_t bins, HashFunc hfunc, InsertFunc ifunc)
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

int HtableFind(Htable * tab, const char * string, char *result)
{
    int bin = tab->hfunc(string, strlen(string)) % tab->bins;
    for (List * lst = tab->table[bin]; lst; lst = lst->nxt)
        if (!strcmp(string, lst->elem)) return HTABLE_FOUND;

    return HTABLE_NOT_FOUND;
}

int64_t ListInsertStud(void * lst, const void * elem)
{
    if (!ListInsert((List*) lst, (const char*) elem))
        return ParseListError(LST_INSERT_ERROR);

    return LST_SUCCESS;
}

int64_t HashFunction(const void * elem, size_t size)
{
    int64_t hash = 5381;
    const char * str = (const char *) elem;
    for (int i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;
}

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
