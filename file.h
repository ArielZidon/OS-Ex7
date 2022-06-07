
/*
** file.h && file.c 
** take from https://www.youtube.com/watch?v=n2AAhiujAqs
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 128
#define BLOCKSIZE 512
#define NAME_SIZE 9
#define MAX_DIR 10
#define BUFF_SIZE 80

typedef struct myopenfile
{
    int fd;
    int pos;
}myopenfile;

typedef struct superblock
{
    int num_inodes;
    int num_blocks;
    int size_blocks;
}superblock;

typedef struct inode
{
    int size;
    int first_block;
    char name[NAME_SIZE+1];
    int dir; //0 file 1 dir
}inode;

typedef struct disk_block
{
    int next_block_num;
    char data[BLOCKSIZE];
}disk_block;

typedef struct mydirent { 
    int size;
    int fds[MAX_DIR];
    char d_name[NAME_SIZE];
}mydirent;

myopenfile open_f [10000];
superblock sb;
inode *inodes;
disk_block *dbs;

/******************MAIN FUNCTION******************/

void mymkfs(int size); // initialize new filesysyem

int mymount(const char *source, const char *target,
const char *filesystemtype, unsigned long
mountflags, const void *data);

int myopen(const char *pathname, int flags);

int myclose(int myfd);

ssize_t myread(int myfd, void *buf, size_t count);

ssize_t mywrite(int myfd, const void *buf, size_t count);

/****************************************/



void mount_fs(const char *source);  // load adile system
void sync_fs(const char *target);   // write the file system
void print_fs(); // print out info about file system
void set_filesize(int filenum, int size);
void write_data(int filenum, int _pos, char data);
char read_data(int filenum, int pos);
int allocte_file(int size,const char* name); // retrun file discriptor
struct mydirent *myreaddir(int fd);