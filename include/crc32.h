#ifndef CRC_32_H
#define CRC_32_H

const unsigned int CRC32INIT = 5381;

unsigned int xcrc32 (const char *buf, int len, unsigned int init);

#endif
