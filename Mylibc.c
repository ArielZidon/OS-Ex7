#include "Mylibc.h"


myFILE *myfopen(const char *restrict pathname, const char *mode){
    struct myFILE* file = (myFILE*)malloc(sizeof(myFILE));
    if(file == NULL){
        perror("Eroor! can't create file");
        return NULL;
    }
    file->id = myopen(pathname, 0);
    if(mode[1] == '+'){
        file->type[0] = mode[0];
        file->type[1] = mode[1];
    }else{
        file->type[0] = mode[0];
    }


    return file;
}

size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream){
    if(stream->type[0] != 'r'){
        perror("Error! you need a access to read the file");
        return -1;
    }
    size_t bytes_requested = size * nmemb;
    size_t bytes_read = myread(stream->id, ptr, bytes_requested);
    return bytes_read / size;
}

int myfseek(myFILE *stream, long offset, int whence){
    if(whence < 0){
        perror("Error!");
        return -1;
    }
    
}
