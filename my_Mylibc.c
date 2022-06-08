#include "my_Mylibc.h"
#include "file.h"


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

size_t myfread(void *restrict ptr, size_t size, size_t nmemb, *restrict stream){

}




int main()
{
    return 0;
}