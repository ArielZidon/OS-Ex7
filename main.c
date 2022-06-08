#include "file.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);

    mymount(NULL,"fs_data",NULL,0,NULL);    
    // mymount("fs_data",NULL,NULL,0,NULL);

    // int x = allocte_file(2400,"ariel");
    int x = myopen("root/ariel", 1);
    char buf [2000];
    int i = 0;
    for(i; i<=20; i++)
    {
        buf[i] = 'a';
    }
    buf[21] = '\0';
    mywrite(x, buf,21);

    myopen("root/afik", 1);

    myopen("root/dvir", 1);

    char res [1000];
    mylseek(x, 0, 0);
    
    // printf("%s",read_data_test(1,0));
    myread(x, res, 100);
    printf("%s\n",res);    
    print_fs();
    // mymount(NULL,"fs_data",NULL,0,NULL);
    printf("done!\n");
    return 0;
}