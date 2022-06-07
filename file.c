#include "file.h"

/*******************FINDES*****************/
int find_empty_inode()
{
    for (int i = 0; i < sb.num_inodes; i++)
    {
        if (inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
}
int find_empty_block()
{
    for (int i = 0; i < sb.num_blocks; i++)
    {
        if (dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}
/************************************************************/

void mymkfs(int size) {
    
    int size_fs= size- sizeof(superblock);

    sb.num_inodes = (size_fs/10)/sizeof(inode);
    sb.num_blocks = (size_fs-sb.num_inodes)/sizeof(disk_block);

    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);


     for (int i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "");
    }
    
    for (int i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
    }
    
    // create_fs();
}

// void create_fs()
// {
//     int size-= sizeof(struct superblock);
//     sb. = (size/10)/(sizeof(struct inode));
//     sb.num_inodes = 10;
//     sb.num_blocks = 100;
//     sb.size_blocks = sizeof(struct disk_block);

//     inodes = malloc(sizeof(struct inode) * sb.num_inodes);
//     for (int i = 0; i < sb.num_inodes; i++)
//     {
//         inodes[i].size = -1;
//         inodes[i].first_block = -1;
//         strcpy(inodes[i].name, "");
//     }
//     dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
//     for (int i = 0; i < sb.num_blocks; i++)
//     {
//         dbs[i].next_block_num = -1;
//     }
//     // creat();
// }

int mymount(const char *source, const char *target,
 const char *filesystemtype, unsigned long
mountflags, const void *data)
{
    if(source==NULL&&target==NULL) 
    {
        return 1;
    }
    if (source!=NULL)
    {
        mount_fs(source);
    }
    if (target!=NULL)
    {
        sync_fs(target);
    }

}

void mount_fs(const char *source)
{
    FILE *file = fopen(source, "r");
    // super block
    fread(&sb, sizeof(struct superblock), 1, file);

    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);

    // inodes
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    // dbs
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}
void sync_fs(const char *target)
{
    FILE *file = fopen("fs_data", "w+");
    // super block
    fwrite(&sb, sizeof(struct superblock), 1, file);
    // inodes
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    // dbs
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}

void write_data(int filenum, int _pos, char data)
{
    int pos = _pos;
    int rb = inodes[filenum].first_block;

    while (pos>=BLOCKSIZE) 
    {
        pos-=BLOCKSIZE;

        if (dbs[rb].next_block_num==-1) {
            perror("dbs[rb].next_block_num==-1");
        } 
        else if (dbs[rb].next_block_num == -2) // the current block is the last block, so we allocate a new block
        { 
            dbs[rb].next_block_num = find_empty_block(); 
            rb = dbs[rb].next_block_num;
            dbs[rb].next_block_num = -2; 
        } 
        else 
        {
            rb = dbs[rb].next_block_num;
        }
    }
    if (_pos>inodes[filenum].size) {
        inodes[filenum].size = _pos+1;
    }
    dbs[rb].data[pos] = data;
}

struct mydirent *myreaddir(int fd) {
    
    if (inodes[fd].dir!=1) 
    {
        perror("inodes[fd].dir!=1");
    }
    return (mydirent*)dbs[inodes[fd].first_block].data;
}


/*************************NEED TO BE CHANGE********************************/
/**************************************************************************/

int myopendir(const char *pathname) {

    char str[BUFF_SIZE];
    strcpy(str, pathname);
    char *token;
    const char s[2] = "/";
    token = strtok(str, s); 
    char currpath[NAME_SIZE] = "";
    char lastpath[NAME_SIZE] = "";
    while(token != NULL ) {          
        if (token != NULL) {
            strcpy(lastpath, currpath);
            strcpy(currpath, token);
        }
        token = strtok(NULL, s);
    }
    for (int i = 0; i < sb.num_inodes; i++)
    {
        if (!strcmp(inodes[i].name, currpath)) {
            if (inodes[i].dir!=1) {
                perror("inodes[i].dir!=1");
                return -1;
            }
            return i; 
        }
    }

    int fd = myopendir(lastpath);
    if (fd==-1) {
        perror("fd==-1");
        return -1;
    }
    if (inodes[fd].dir!=1) {
        perror("inodes[fd].dir!=1");
        return -1;
    }
    int dirblock = inodes[fd].first_block;
    struct mydirent* currdir = (struct mydirent*)dbs[dirblock].data;
    if (currdir->size>=MAX_DIR) {
        perror("currdir->size>=MAX_DIR");
        return -1;  
    }

    int newdirfd = allocte_file(sizeof(struct mydirent), currpath);
    currdir->fds[currdir->size++] = newdirfd;
    inodes[newdirfd].dir = 1;
    struct mydirent* newdir = malloc(sizeof(struct mydirent));
    newdir->size = 0;
    for (size_t i = 0; i < MAX_DIR; i++)
    {
        newdir->fds[i] = -1;
    }
    
    char *newdiraschar = (char*)newdir;

    for (size_t i = 0; i < sizeof(struct mydirent); i++)
    {
        write_data(newdirfd, i, newdiraschar[i]);        
    }
    strcpy(newdir->d_name, currpath);
    return newdirfd;
}

int creatf(const char *path, const char* name) {
    
    int newfd = allocte_file(1,name);
    int dirfd = myopendir(path);
    struct mydirent *currdir = myreaddir(dirfd);
    currdir->fds[currdir->size++] = newfd;
    return newfd;
}

int myopen(const char *pathname, int flags) {

    char str[BUFF_SIZE];
    strcpy(str, pathname);
    char *token;
    const char s[2] = "/";
    token = strtok(str, s); 
    char currpath[NAME_SIZE] = "";
    char lastpath[NAME_SIZE] = "";
    while(token != NULL ) {          
        if (token != NULL) {
            strcpy(lastpath, currpath);
            strcpy(currpath, token);
        }
        token = strtok(NULL, s);
    }
    for (size_t i = 0; i < sb.num_inodes; i++)
    {
        if (!strcmp(inodes[i].name, currpath)) {
            if (inodes[i].dir!=0) {
                return -1;
            }
            open_f[i].fd = i;
            open_f[i].pos = 0;
            return i; 
        }
    }
    int i = creatf(lastpath, currpath); 
    open_f[i].fd = i;
    open_f[i].pos = 0;
    return i;
}

int allocte_file(int size,const char* name)
{
    if (strlen(name)>7) {
        perror ("strlen(name)>7");
        return -1;
    }
    int inode = find_empty_inode(); 

    if (inode == -1) {
        perror ("inode == -1");
        return -1;
    }
    int curr_block = find_empty_block(); 

    if (curr_block == -1) {
        perror ("curr_block == -1");
        return -1;
    }
    inodes[inode].size = size;
    inodes[inode].first_block = curr_block;
    dbs[curr_block].next_block_num = -2;

    strcpy(inodes[inode].name, name);

    if (size>BLOCKSIZE) {  
        int allocated_size = -(3*BLOCKSIZE)/4;
        
        int next_block;
        while (allocated_size<size)
        {
            next_block = find_empty_block();
            if (next_block == -1) {
                perror("next_block == -1");
                return -1;
            }
            dbs[curr_block].next_block_num = next_block;
            curr_block = next_block;
            allocated_size+=BLOCKSIZE;
        }
    }
    dbs[curr_block].next_block_num = -2;

    return inode;
}
/**************************************************************************/
/**************************************************************************/


void print_fs()
{
    printf("superblock info\n");
    printf("num_inodes %d\n", sb.num_inodes);
    printf("num_blocks %d\n", sb.num_blocks);
    printf("size_blocks %d\n\n", sb.size_blocks);

    printf("inodes:\n");
    for (int i = 0; i < sb.num_inodes; i++)
    {
        printf("name %s ", inodes[i].name);
        printf("size %d  ", inodes[i].size);
        printf("first_block %d\n", inodes[i].first_block);
    }
    // dbs
    printf("block:\n");
    for (int i = 0; i < sb.num_blocks; i++)
    {
        printf("block num: %d next block %d\n", i, dbs[i].next_block_num);
    }
}



void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if(dbs[bn].next_block_num >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}
void set_filesize(int filenum, int size)
{
    int temp = size + BLOCKSIZE -1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    //grow the file if necessary
    for (num-- ; num > 0; num--)
    {
        //check next block number
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2)
        {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
    }
    //short the file if necessary 
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
}

int get_block_num(int file, int offeset)
{
    int bn = inodes[file].first_block;
    for(int togo = offeset; togo > 0; togo--)
    {
        bn = dbs[bn].next_block_num;
    }
    return bn;
}



char* read_data(int filenum, int pos)
{
    //calculate witch block
    int relative_block = pos/BLOCKSIZE;
    //find the block number
    int bn = get_block_num(filenum, relative_block);
    //calculate the offset in the block
    int offset = pos % BLOCKSIZE;
    //read the data
    char* res = malloc(sizeof(char)*1024);
    strcpy(res, &(dbs[bn].data[offset]));
}