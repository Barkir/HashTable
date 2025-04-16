#ifndef IO_H
#define IO_H

enum IO_FLAGS
{
    IO_ALIGNED,
    IO_DEFAULT
};

const size_t NUM_WORDS = 2000;
const size_t BUF_LEN = 16;
const size_t ALIGNED_SIZE = 65536 * 128;

int ProcessCmd(int argc, char * argv[]);
int File2Lines(int a_flag, char *** buf, const char * filename);
#endif
