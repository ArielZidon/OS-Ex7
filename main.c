#include "file.h"

int main(int argc, char const *argv[])
{
    // mymkfs(10000);

    // mymount(NULL,"fs_data",NULL,0,NULL);    
    mymount("fs_data",NULL,NULL,0,NULL);

    // int x = allocte_file(2400,"ariel");
    int x = myopen("folder/ariel", 1);
    
    mywrite(x,"WELLCOM TO AFIK AND ARIEL UFS!!!\n",35);

    myopen("folder/afik", 1);

    myopen("folder/dvir", 1);

    char res [1000];
    mylseek(x, 0, 0);
    
    myread(x, res, 100);
    printf("%s\n",res);    
    print_fs();
    // mymount(NULL,"fs_data",NULL,0,NULL);
    printf("done!\n");
    return 0;
}