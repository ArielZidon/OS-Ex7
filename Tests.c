#include "file.h"
#include "Mylibc.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);
    mymount(NULL,"fs_data",NULL,0,NULL); 

    /***********************TEST1**************************/

    int y = myopen("folder/afik", 1);
    
    mywrite(y,"THE READ AND WRITE IS WORK!!!",30);

    mylseek(y, 0, 0);    

    char buf [100];
    myread(y, buf, 50);
    printf("%s\n",buf);

    myclose(y);
    printf("****************************************FILE SYSTEM****************************************\n\n\n");
    print_fs();
    printf("\n\n");
}