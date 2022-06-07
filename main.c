#include "file.h"

int main(int argc, char const *argv[])
{
    // mymkfs(10000);

    // mymount(NULL,"fs_data",NULL,0,NULL);
    mymount("fs_data",NULL,NULL,0,NULL);

    int x = allocte_file(1000,"ariel");

    myopen("ariel", 100);

    char buf[100];
    myread(x, buf, 10);
    printf("%s\n",buf);

    print_fs();
    printf("done!\n");
    return 0;
}