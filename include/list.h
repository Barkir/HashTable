#ifndef LIST_H
#define LIST_H


typedef int64_t (*CompareFunc) (const void *, const void *);

struct List
{
    char * elem;
    List * nxt;
    CompareFunc cmp;
};

int ListInsert(List * lst, const char * str);
int ListCtor(List * lst);
int64_t ListCompare(const void * el1, const void * el2);

#endif
