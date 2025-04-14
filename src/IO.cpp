#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "IO.h"
#include "errors.h"



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

    *buf = (char**) calloc(fsize + 1, sizeof(char));
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
        line[BUF_LEN] = 0;
        (*buf)[count++] = strdup(line);
    }

    fclose(fp);
    return 0;
}

