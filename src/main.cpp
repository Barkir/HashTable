#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "list.h"
#include "htable.h"
#include "errors.h"

const size_t BUF_LEN = 16;
int File2Lines(char *** buf, const char * filename);

int main(void)
{
    Htable * tab = NULL;
    if (HtableInit(&tab, HTABLE_BINS, HashFunction, ListInsertStud))
        return ParseHtableError(HTABLE_INIT_ERROR);


    char ** lines = NULL;
    File2Lines(&lines, "parsed.lib");
    for (int i = 0; i < 2000; i++)
    {
        HtableInsert(tab, lines[i]);
    }

    HtableDump(tab);


program_exit:
    HtableDestroy(tab);
    return 0;
}

int File2Lines(char *** buf, const char * filename)
{
    FILE * fp = fopen(filename, "r+");
    if (!fp)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    struct stat stats;
    if (fstat(fileno(fp), &stats) == -1)
    {
        perror("fstat");
        fclose(fp);
        return EXIT_FAILURE;
    }
    size_t fsize = stats.st_size;

    *buf = (char**) calloc(fsize + 10, sizeof(char));
    if (!buf)
    {
        perror("book allocation");
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (setvbuf(fp, **buf, _IOFBF, fsize))
    {
        perror("setvbuf");
        free(*buf);
        fclose(fp);
        return EXIT_FAILURE;
    }

    char line[BUF_LEN + 2] = "";
    int count = 0;
    while (fgets(line, BUF_LEN + 2, fp))
    {
        (*buf)[count++] = strdup(line);
    }


    fclose(fp);
    return 0;
}
