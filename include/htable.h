#ifndef HTABLE_H
#define HTABLE_H

struct Htable
{
    size_t cap;
    size_t sz;

    List ** table;
};


#endif
