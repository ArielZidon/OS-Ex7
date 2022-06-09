#include "Mylibc.h"
#include "file.h"

// myFILE *myfopen(const char *restrict pathname, const char *mode)
// {
//     if((strcmp(mode, "a")&&strcmp(mode, "w")&&strcmp(mode, "r")&&strcmp(mode, "r+"))) 
//     {
//         perror("Not a legal mode");
//     }

//     struct myFILE* file = (myFILE*)malloc(sizeof(myFILE));
//     if(file == NULL)
//     {
//         perror("Error! can't create file");
//         return NULL;
//     }

//     file->id = myopen(pathname, 0);
//     if(file->id == -1)
//     {
//         perror("file->id == -1");
//     }
//     if(mode[1] == '+')
//     {
//         file->type[0] = mode[0];
//         file->type[1] = mode[1];
//     }
//     else
//     {
//         file->type[0] = mode[0];
//     }
//     file->type[3] = '\0';

//     if(!(strcmp(mode, "a")))
//     {
//         file->pos = 0;
//     }
//     else
//     {
//         file->pos =inodes[file->id].size;
//     }
//     file->size =inodes[file->id].size;

//     file->data = (char*)malloc(sizeof((file->size)+1));
//     if (!(strcmp(mode, "w"))) { 
//         for (int i = 0; i < file->size; i++)
//         {
//             file->data[i] = ' ';
//         }
//         file->data[file->size] = '\0';
//     }
//     else
//     {
//         for (int i = 0; i < file->size; i++)
//         {
//             file->data[i] = read_data(file->id,i);
//         }
//     }
//     myclose(file->id);
//     return file;
// }

myFILE* myfopen(const char *pathname, const char *mode) 
{
    if((strcmp(mode, "a")&&strcmp(mode, "w")&&strcmp(mode, "r")&&strcmp(mode, "r+"))) //checking legal mode
    {
        perror("Not a legal mode");
        return NULL;
    }

    int id = myopen(pathname, 0);
    if (id==-1) 
    {
        perror("Error! can't create file");
        return NULL;
    }
    
    myFILE* res = (myFILE*)malloc(sizeof(myFILE));// allocate myFILE 
    if(mode[1] == '+')
    {
        res->type[0] = mode[0];
        res->type[1] = mode[1];
    }
    else
    {
        res->type[0] = mode[0];
    }
    res->type[3] = '\0';
    res->id = id;
    res->size = inodes[id].size;
    if (!strcmp(mode, "a")) //set the files' pos 
    { 
        res->pos = inodes[id].size;
    } 
    else 
    {
        res->pos = 0;
    }
    res->data = malloc(res->size);
    if (!strcmp(mode, "w")) //if we want to write ('w') we want to clenn up the data
    { 
        for (int i = 0; i < res->size; i++)
        {
            res->data[i] = ' ';
        }
        res->data[res->size] = '\0';
    } 
    else // write the data
    {
        for (int i = 0; i < res->size; i++)
        {
            res->data[i] = read_data(id, i);
        }
        res->data[res->size+1] = '\0';
    }
    myclose(id);
    return res;
}

int myfclose(myFILE *stream)
{
    if(stream->type[0] != 'r')
    {
        free(stream->data);
        free(stream);
    }
    else
    {
        for(int i = 0; i < stream->size ; i++)
        {
            write_data(stream->id, i, stream->data[i]);
        }
        free(stream->data);
        free(stream);
    }
    return 0;

}


/**********************************tomorrow*************************************/
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if(stream->type[0] != 'r')
    {
        perror("Error! you need a access to read the file");
        return -1;
    }

    int bytes_requested = size * nmemb;
    char *res = malloc(bytes_requested + 1);
    if(res == NULL)
    {
        perror("res has faild!");
        return 1;
    }
    int pos = stream->pos;

    for(int i =0 ;i < bytes_requested ; i++)
    {
        if (stream->pos+i < stream->size) 
        {
            res[i] = stream->data[stream->pos+i];
            pos++;
        }
    }
    res[bytes_requested] = '\0';
    stream->pos = pos;
    strncpy(ptr,res, bytes_requested); //get into ptr..
    free(res);
    return stream -> pos;
}

size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if(stream->type[0] != 'w')
    {
        perror("Error! you need a access to read the file");
        return -1;
    }
    size_t bytes_provide = size * nmemb;
    char *res = (char*)ptr;

    if(stream->pos+bytes_provide > stream->size)
    {
        char* buf = malloc(stream->size+1);
        for(int i = 0 ; i < stream->size ; i++)
        {
            buf[i] = stream->data[i];
        }
        free(stream->data);
        stream->data = malloc(stream->pos + bytes_provide);
        for(int i = 0 ; i < stream->size ; i++)
        {
            stream->data[i] = buf[i];
        }
        free(buf);
    }
    int ptrT = stream->pos;
    for (int i = 0; i < bytes_provide; i++)
    {
        stream->data[stream->pos+i] = res[i];
        ptrT++;
    }
    stream->pos = ptrT;
    return stream->pos;
}

/******************************************************************************/

int myfseek(myFILE *stream, long offset, int whence)
{
    if(whence == SEEK_SET) 
    {
        stream->pos = offset;
    }
    else if(whence == SEEK_CUR) 
    {
        stream->pos += offset;
    }
    else if (whence == SEEK_END) 
    {
        stream->pos = stream->size+offset;
    } 
    return stream->pos;
}

int myfprintf(myFILE *restrict stream, const char *restrict format, ...){
    int sum = 0, i = 0;
    va_list arguments;                     
    va_start(arguments, format);    
    sum = va_arg(arguments, int);  
    char c = va_arg(arguments, char);
    float f = va_arg(arguments, float);
    char trash = va_arg(arguments, char);
    for(i = 0; i < (int)strlen(format); i++){

        if(format[i] == '%')
        {
            if(format[i + 1] == 'd')
            {
                sum = va_arg(arguments, int);
                myfwrite(&sum, sizeof(int), 1, stream);
            }

            if(format[i + 1] == 'f')
             {
                f = va_arg(arguments, float);
                myfwrite(&f, sizeof(float), 1, stream);
             }

            if(format[i + 1] == 'c')
             {
                c = va_arg(arguments, int);
                myfwrite(&c, sizeof(char), 1, stream);
             }
        }
        else{
            for(; (format[i] != '%'); i++)
            {
                trash = va_arg(arguments, char);
                myfwrite(&trash, sizeof(char), 1, stream);
            }
        }
    }
    return i;
}

int myfscanf(myFILE *restrict stream, const char *restrict format, ...)
{
    int i;
    va_list arguments;
    va_start(arguments, format);
    int sum = va_arg(arguments, int);  
    char c = va_arg(arguments, char);
    float f = va_arg(arguments, float);
    
    for (i = 0; i < (int)strlen(format); i++)
    {
        if (format[i] == '%')
        {
            if(format[i + 1] == 'd'){
                sum = va_arg(arguments, int);
                myfread(&sum, sizeof(int), 1, stream);
            }
            if(format[i + 1] == 'f'){
                f = va_arg(arguments, float);
                myfread(&f, sizeof(float), 1, stream);
            }
            if (format[i + 1] == 'c'){
                c = va_arg(arguments, char);
                myfread(&c, sizeof(char), 1, stream);
            }
        }
    }
    return i;
}
