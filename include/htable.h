#ifndef HTABLE_H
#define HTABLE_H



typedef int64_t (*HashFunc) (const void *, size_t);
typedef int64_t (*InsertFunc) (void *, const void *);

struct Htable
{
    size_t bins;
    size_t capacity;

    List ** table;
    HashFunc hfunc;
    InsertFunc ifunc;
};

int HtableInit(Htable ** tab, size_t bins, HashFunc hfunc, InsertFunc ifunc);
int HtableFind(Htable * tab, const char * string, char *result);
int HtableDestroy(Htable * tab);
int HtableInsert(Htable * tab, const char * string);
int HtableDump(Htable * tab);
int64_t HashFunction(const void * elem, size_t size);
int64_t ListInsertStud(void * lst, const void * elem);
float HashLoadFactor(Htable * tab);

const int64_t HTABLE_BINS = 128;


#endif
