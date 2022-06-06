#include "file.h"
#include<string.h>

superblock sb;
inode *inodes;
disk_block *dbs;

//creat fs
void mymkfs()
{
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof(disk_block);

    inodes = malloc(sizeof(inode) * sb.num_inodes);

    for(int i = 0; i<sb.num_inodes;i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name,"");
    }

    dbs = malloc(sizeof(disk_block) * sb.num_blocks);

    for(int i = 0; i<sb.num_blocks;i++)
    {
        dbs->next_block_num = -1;
    }
}

//initialize new fs
int mymount()
{
    FILE *file = fopen("fs_data","r");

    //superBlock
    fread(&sb,sizeof(superblock),1,file);

    //inodes
    inodes = malloc(sizeof(inode) * sb.num_inodes);
    fread(inodes,sizeof(inode),sb.num_inodes,file);

    //blocks
    dbs = malloc(sizeof(disk_block) * sb.num_blocks);
    fread(dbs,sizeof(disk_block),sb.num_blocks,file);
    fclose(file);
}

//write the fs
void fync_fs()
{
    FILE *file = fopen("fs_data","w+");

    //superBlock
    fwrite(&sb,sizeof(superblock),1,file);

    //inodes
    fwrite(inodes,sizeof(inode),sb.num_inodes,file);

    //blocks
    fwrite(dbs,sizeof(disk_block),sb.num_blocks,file);
    
    fclose(file);
}

void print_fs()
{
    printf("super_block info:\n");
    printf("\tnum_inodes: %d\n",sb.num_inodes);
    printf("\tnum_blockes: %d\n",sb.num_blocks);
    printf("\tsize_blockes: %d\n",sb.size_blocks);

      for(int i = 0; i<sb.num_inodes;i++)
    {
        printf("\t name: %s\n",inodes[i].name);
    }

    for(int i = 0; i<sb.num_blocks;i++)
    {
        printf("\tblock number: %d, next block: %d",i,dbs->next_block_num);
    }

}

int alocate_file(char name [8])
{
    int in = find_empty_inode();
    strcpy(inodes[in].name,name);
    int block = find_empty_block();
    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

}

int find_empty_inode()
{
    for(int i=0;i<sb.num_inodes;i++)
    {
        if(inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
}

int find_empty_block()
{
    for(int i=0;i<sb.num_blocks;i++)
    {
        if(dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}
