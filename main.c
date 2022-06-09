#include "file.h"
#include "Mylibc.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);

    mymount(NULL,"fs_data",NULL,0,NULL);    
    // mymount("fs_data",NULL,NULL,0,NULL);

    int x = myopen("folder/ariel", 1);
    
    mywrite(x,"WELLCOM TO AFIK AND ARIEL UFS!!!\n",35);

    myopen("folder/afik", 1);

    myopen("folder/dvir", 1);

    char res [1000];
    mylseek(x, 0, 0);
    
    // printf("%s",read_data_test(1,0));
    // myread(x, res, 100);
    // printf("%s\n",res);    
    // print_fs();
    // mymount(NULL,"fs_data",NULL,0,NULL);

    /************************AFIK***************************/

    myFILE *ariel =  myfopen("folder/ariel", "r");

    myfread(res, 31 , 1 , ariel);
    printf("%s\n",res);

    myfclose(ariel);

    printf("done!\n");
    return 0;
}