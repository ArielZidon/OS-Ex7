#include "file.h"

int main(int argc, char const *argv[])
{
    // mymkfs(10000);

    // mymount(NULL,"fs_data",NULL,0,NULL);
    mymount("fs_data",NULL,NULL,0,NULL);

    int x = allocte_file(2400,"ariel");
    myopen("ariel", 100);

    int y = allocte_file(1200,"afik");
    myopen("afik", 100);

    mylseek(x, 5, 10);

    char buf[100];
    myread(x, buf, 10);
    printf("%s\n",buf);

    myread(y, buf, 10);
    printf("%s\n",buf);

    print_fs();
    printf("done!\n");
    return 0;
}