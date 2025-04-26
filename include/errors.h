#ifndef ERRORS_H
#define ERRORS_H

// #define DEBUG

#ifdef DEBUG
#define LOGGER(...) \
    fprintf(stdout, "(%s, %s) %d: ", __FILE__, __func__, __LINE__);     \
    fprintf(stdout, __VA_ARGS__);                                       \
    fprintf(stdout, "\n")
#endif
#ifndef DEBUG
#define LOGGER(...)
#endif

enum HtableError
{
    HTABLE_SUCCESS,
    HTABLE_MEMALLOC_ERROR,
    HTABLE_FILE_OPEN_ERROR,
    HTABLE_INSERT_ERROR,
    HTABLE_INIT_ERROR,
    HTABLE_UNKNOWN_ERROR,

    HTABLE_FOUND,
    HTABLE_NOT_FOUND
};

enum ListError
{
    LST_SUCCESS,
    LST_MEMALLOC_ERROR,
    LST_FILE_OPEN_ERROR,
    LST_INSERT_ERROR,
    LST_INIT_ERROR,
    LST_UNKNOWN_ERROR
};

enum ListError ParseListError(enum ListError error);
enum HtableError ParseHtableError(enum HtableError error);

#endif
