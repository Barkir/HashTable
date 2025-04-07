#ifndef LIST_H
#define LIST_H

struct List
{
    char * elem;
    List * nxt;
};

enum ListErrros
{
    LST_SUCCESS,
    LST_MEMALLOC_ERROR,
}

int ListInsert(List * lst, char * str);
int ListCtor(List * lst);

#endif
