#ifndef IO_H
#define IO_H

const size_t NUM_WORDS = 2000;
const size_t BUF_LEN = 32;

int File2Lines(char *** buf, const char * filename);
int CountSymbol(char * text, char sym);

#endif
