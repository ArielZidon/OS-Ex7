#include "Mylibc.h"
#include "file.h"

myFILE *myfopen(const char *restrict pathname, const char *mode)
{
    if((strcmp(mode, "a")&&strcmp(mode, "w")&&strcmp(mode, "r")&&strcmp(mode, "r+"))) 
    {
        perror("Not a legal mode");
    }

    struct myFILE* file = (myFILE*)malloc(sizeof(myFILE));
    if(file == NULL)
    {
        perror("Error! can't create file");
        return NULL;
    }

    file->id = myopen(pathname, 0);
    if(file->id == -1)
    {
        perror("file->id == -1");
    }
    if(mode[1] == '+')
    {
        file->type[0] = mode[0];
        file->type[1] = mode[1];
    }
    else
    {
        file->type[0] = mode[0];
    }
    file->type[3] = '\0';

    if(!(strcmp(mode, "a")))
    {
        file->pos = 0;
    }
    else
    {
        file->pos =inodes[file->id].size;
    }
    file->size =inodes[file->id].size;

    file->data = (char*)malloc(sizeof((file->size)+1));
    if (!(strcmp(mode, "w"))) { 
        for (int i = 0; i < file->size; i++)
        {
            file->data[i] = ' ';
        }
        file->data[file->size] = '\0';
    }
    else
    {
        for (int i = 0; i < file->size; i++)
        {
            file->data[i] = read_data(file->id,i);
        }
    }
    myclose(file->id);
    return file;
}

size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if(stream->type[0] != 'r')
    {
        perror("Error! you need a access to read the file");
        return -1;
    }
    size_t bytes_requested = size * nmemb;
    size_t bytes_read = myread(stream->id, ptr, bytes_requested);
    return bytes_read / size;
}

int myfseek(myFILE *stream, long offset, int whence)
{
    if(whence < 0)
    {
        perror("Error!");
        return -1;
    }
    int res = mylseek(stream->id, offset, whence);
    return 0;
}
