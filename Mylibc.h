#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct myFILE
{
    int id;
    char* data;
    int size;
    char type[3];
    int pos;
}myFILE;


myFILE *myfopen(const char *restrict pathname, const char *mode);
int myfclose(myFILE *stream);
int myfseek(myFILE *stream, long offset, int whence);
// int myfscanf(myFILE *restrict stream, const char format, ...);
// int myfprintf(myFILE *restrict stream, const char format, ...);
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);
// size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);