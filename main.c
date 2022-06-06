#include "file.h"

int main()
{
    mymkfs();
    fync_fs();
    mymount();
    alocate_file("dvir");
    print_fs();
    printf("done\n");
    return 0;
}