#include "file.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);
    mymount(NULL,"fs_data",NULL,0,NULL);
    // mymount("fs_data",NULL,NULL,0,NULL);
    allocte_file(1000,"ariel");
    myopen("ariel", 100);
    print_fs();
    printf("done!\n");
    return 0;
}