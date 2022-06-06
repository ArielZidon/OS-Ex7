#include "file.h"

int main(int argc, char const *argv[])
{
    // create_fs();
    // sync_fs();
    mount_fs();
    int d = allocte_file("ariel");
    int a = allocte_file("zidon");
    set_filesize(0, 5000);
    write_data(d, 0, "afik ia homo!!!");
    printf("%s\n", read_data(d, 0));
    print_fs();
    sync_fs();
    printf("done!\n");
    return 0;
}