#include<stdio.h>
#include<stdlib.h>

#define PATH_MAX 50
#define BLOCKSIZE 512
#define MAX_FILES 1000

// typedef struct dirent {
//     /* File position within stream */
//     long d_off;
//     /* File name */
//     char d_name[PATH_MAX];
// }dirent;

// typedef struct myDIR {
//     struct dirent ent;
//     struct _WDIR *wdirp;
// }myDIR;

typedef struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
}superblock;

typedef struct inode {
    int size;
    int first_block;
    char name[10];
}inode;

typedef struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
}disk_block;

//creat fs
void mymkfs();

//initialize new fs
int mymount();

//write the fs
void fync_fs();

void print_fs();

int alocate_file(char name [8]);

int find_empty_inode();

int find_empty_block();


// int myopen(const char *pathname, int flags);
// int myclose(int myfd);
// ssize_t myread(int myfd, void *buf, size_t count);
// ssize_t mywrite(int myfd, const void *buf, size_t count);
// off_t mylseek(int myfd, off_t offset, int whence);
// myDIR *myopendir(const char *name);
// struct mydirent *myreaddir(myDIR *dirp);
// int myclosedir(myDIR *dirp);