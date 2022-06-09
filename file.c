#include "file.h"

/**************************FINDES*****************************/
/*************************************************************/
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
/*************************************************************/
/*************************************************************/


void mymkfs(int size) 
{
    int i = 0;
    int size_fs= size- sizeof(superblock);
    sb.num_inodes = (size_fs/10)/sizeof(inode);
    sb.num_blocks = (size_fs-sb.num_inodes)/sizeof(disk_block);
    sb.size_blocks = BLOCKSIZE;
    
    inodes = malloc(sizeof(inode) * sb.num_inodes);
    dbs = malloc(sizeof(disk_block) * sb.num_blocks);

    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "");
    }
    
    for (i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
    }
    
    int buff = create_file(sizeof(struct mydirent), "folder");
    if (buff != 0 ) 
    {
        perror("Error problem in create_file");
        return;
    }

    inodes[buff].dir = 1;
    struct mydirent* curr = malloc(sizeof(struct mydirent));
    if(curr == NULL)
    {
        perror("Error to create - mydirent*");
        return;
    }

    curr->size = 0;
    for (i = 0; i < MAX_DIR; i++)
    {
        curr->fds[i] = -1;        
    }

    strcpy(curr->d_name, "folder");
    char* c_dir = (char*)curr;
    for (i = 0; i < sizeof(struct mydirent); i++)
    {
        write_data(buff, i, c_dir[i]);
    }
    free(curr);
}

int mymount(const char *source, const char *target,
 const char *filesystemtype, unsigned long
mountflags, const void *data)
{
    if(source==NULL&&target==NULL) 
    {
        printf("source==NULL && target==NULL....");
        return 1;
    }
    if (target!=NULL)
    {
        sync_fs(target);
    }
    if (source!=NULL)
    {
        mount_fs(source);
    }
}

void mount_fs(const char *source)
{
    FILE *file = fopen(source, "r");
    // super block
    fread(&sb, sizeof(superblock), 1, file);

    // inodes
    inodes = malloc(sizeof(inode) * sb.num_inodes);
    fread(inodes, sizeof(inode), sb.num_inodes, file);

    // dbs
    dbs = malloc(sizeof(disk_block) * sb.num_blocks);
    fread(dbs, sizeof(disk_block), sb.num_blocks, file);
    fclose(file);
}

void sync_fs(const char *target)
{
    FILE *file = fopen("fs_data", "w+");
    // super block
    fwrite(&sb, sizeof(superblock), 1, file);
    // inodes
    fwrite(inodes, sizeof(inode), sb.num_inodes, file);
    // blocks
    fwrite(dbs, sizeof(disk_block), sb.num_blocks, file);
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

    if(_pos>inodes[filenum].size) 
    {
        inodes[filenum].size = _pos+1;
    }
    dbs[rb].data[pos] = data;
}

struct mydirent *myread_dir(int fd) {
    
    if (inodes[fd].dir!=1) 
    {
        perror("inodes[fd].dir!=1");
    }
    return (mydirent*)dbs[inodes[fd].first_block].data;
}

/********************************CREATE UFS********************************/
/**************************************************************************/

int Myopen_dir(const char *pathname) 
{
    char str[BUFF_SIZE];
    strcpy(str, pathname);
    char *choset;
    const char mak[2] = "/";
    choset = strtok(str, mak); 
    char this_p[NAME_SIZE] = "";
    char last_p[NAME_SIZE] = "";

    while(choset != NULL ) 
    {          
        strcpy(last_p, this_p); //if?
        strcpy(this_p, choset);
        choset = strtok(NULL, mak);
    }

    for (int i = 0; i < sb.num_inodes; i++)
    {
        if (!strcmp(inodes[i].name, this_p)) // 1 == used dir 
        {
            if (inodes[i].dir!=1) 
            {
                perror("inodes[i].dir!=1");
                return -1;
            }
            return i; 
        }
    }

    int fd = Myopen_dir(last_p);
    if (fd==-1) 
    {
        perror("fd==-1");
        return -1;
    }

    if (inodes[fd].dir!=1) 
    {
        perror("inodes[fd].dir!=1");
        return -1;
    }

    int db = inodes[fd].first_block;
    mydirent* live_d = (mydirent*)dbs[db].data;
    if (live_d->size>=MAX_DIR) 
    {
        perror("live_d->size>=MAX_DIR");
        return -1;  
    }

    int dir = create_file(sizeof(mydirent), this_p);
    live_d->fds[live_d->size++] = dir;
    inodes[dir].dir = 1;
    mydirent* newdir = malloc(sizeof(mydirent));
    newdir->size = 0;
    for (size_t i = 0; i < MAX_DIR; i++)
    {
        newdir->fds[i] = -1;
    }
    
    char *newdiraschar = (char*)newdir;
    for (size_t i = 0; i < sizeof(mydirent); i++)
    {
        write_data(dir, i, newdiraschar[i]);        
    }
    strcpy(newdir->d_name, this_p);
    return dir;
}

int creatf(const char *path, const char* name) 
{
    int fd = create_file(1,name);
    int dir = Myopen_dir(path);
    mydirent *live_d = myread_dir(dir);
    live_d->fds[live_d->size++] = fd;
    return fd;
}

int myopen(const char *pathname, int flags)  // how i want to open the file. flags = r/w/a. 
{
    char buf[BUFF_SIZE];
    strcpy(buf, pathname);
    char *choset;
    const char s[2] = "/";
    choset = strtok(buf, s); 
    char this_p[NAME_SIZE] = "";
    char last_p[NAME_SIZE] = "";

    while(choset != NULL ) 
    {          
        strcpy(last_p, this_p); //if?
        strcpy(this_p, choset);
        choset = strtok(NULL, s);
    }

    for (int i = 0; i < sb.num_inodes; i++)
    {
        if (!strcmp(inodes[i].name, this_p)) 
        {
            if (inodes[i].dir!=0) 
            {
                perror("inodes[i].dir!=0");
                return -1;
            }
            open_f[i].fd = i;
            open_f[i].pos = 0;
            return i; 
        }
    }
    int i = creatf(last_p, this_p); 
    open_f[i].fd = i;
    open_f[i].pos = 0;
    return i;
}

int create_file(int size,const char* name)
{
    int inode = find_empty_inode(); 
    if (inode == -1) 
    {
        perror ("file cant be used");
        return -1;
    }

    int curr_block = find_empty_block(); 
    if (curr_block == -1) 
    {
        perror ("curr_block == -1");
        return -1;
    }
    inodes[inode].size = size;
    inodes[inode].first_block = curr_block;
    dbs[curr_block].next_block_num = -2;

    if (strlen(name)>8) 
    {
        perror ("strlen(name)>8");
        return -1;
    }
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
/******************************CLOSE FD************************************/
/**************************************************************************/
int myclose(int myfd) {
    open_f[myfd].fd = -1;
    open_f[myfd].pos = -1;
}
/******************************READ FROM FILE*****************************/
/*************************************************************************/
char read_data(int filenum, int pos)
{
    int b = inodes[filenum].first_block;
    while (pos >= BLOCKSIZE) 
    {
        pos = pos - BLOCKSIZE;
        b = dbs[b].next_block_num;
        if (b == -1 || b == -2) 
        {
            perror("The block is empty");
            return -1;
        } 
    }
    return dbs[b].data[pos];
}

ssize_t myread(int myfd, void *buf, size_t count)
{

    if (inodes[myfd].dir == 1 || open_f[myfd].fd != myfd) 
    {
        perror("The DIR is empty");
        return -1;
    }

    char* res = malloc(count+1);
    for (int i = 0; i < count; i++)
    {
        res[i] = read_data(myfd, open_f[myfd].pos);
        open_f[myfd].pos++;
    }
    res[count] = '\0';
    strncpy(buf, res, count);
    free(res);

    return open_f[myfd].pos; 
}

/***************************WRITE TO FILE***************************/
/*******************************************************************/
ssize_t mywrite(int myfd, const void *buf, size_t count) 
{

    if (inodes[myfd].dir == 1 || open_f[myfd].fd != myfd) 
    {
        perror("The DIR is empty");
        return -1;
    }
    char* buffer = (char*)buf;
    for (int i = 0; i < count; i++)
    {
        write_data(myfd, open_f[myfd].pos, buffer[i]);
        open_f[myfd].pos++;
    }
    return open_f[myfd].pos;
}
/**************************SEEKING********************************/
/*****************************************************************/
off_t mylseek(int myfd, off_t offset, int whence)
{
    if(open_f[myfd].fd != myfd) 
    {
        perror("Not the current file!");
        return -1;
    }

    if(whence == SEEK_SET) 
    {
        open_f[myfd].pos = offset;
    }
    else if (whence==SEEK_CUR) 
    {
        open_f[myfd].pos = open_f[myfd].pos + offset;
    }
    else if (whence==SEEK_END) 
    {
        open_f[myfd].pos = inodes[myfd].size+offset;
    }  

    if (open_f[myfd].pos<0) 
    {
        open_f[myfd].pos = 0;
    }
    return open_f[myfd].pos;
}
/**************************OPEN DIR*******************************/
/*****************************************************************/

myDIR *myopendir(const char *name)
{
    int path = Myopen_dir(name);
    myDIR *res = (myDIR*)malloc(sizeof(myDIR));
    if(res == NULL)
    {
        perror("Error creat res");
    }
    res->path = path;
    return res;
}

struct mydirent *myreaddir(myDIR *dirp)
{
    mydirent *res = (mydirent*)malloc(sizeof(mydirent));
    res = myread_dir(dirp->path);
    return res;
}

int myclosedir(myDIR *dirp)
{
    printf("Dir %d has been closed",dirp->path);
    return 0;
}

/*****************************************************************/
/*****************************************************************/
void print_fs()
{
    printf("superblock info\n");
    printf("num_inodes %d\n", sb.num_inodes);
    printf("num_blocks %d\n", sb.num_blocks);
    printf("size_blocks %d\n\n", sb.size_blocks);

    printf("folders: \n\n");
    for (int i = 0; i < sb.num_inodes; i++)
    {
        if(!(strcmp(inodes[i].name,"folder")))
        {
            
            printf(" | name %s ", inodes[i].name);
            printf("size %d  ", inodes[i].size);
            printf("first_block: %d\t", inodes[i].first_block);
        }
        
    }
    printf("\n\n");
    printf("inodes:\n");
    for (int i = 0; i < sb.num_inodes; i++)
    {
        if(strcmp(inodes[i].name,"folder"))
        {
        printf("\tname %s ", inodes[i].name);
        printf("size %d  ", inodes[i].size);
        printf("first_block %d", inodes[i].first_block);
        if(i % 4 == 0)
        {
            printf("\n\n");
        }
        else
        {
            printf("   |   ");
        }
        }
    }
    // dbs
    printf("\n");
    printf("block:\n");
    for (int i = 0; i < sb.num_blocks; i++)
    {   
        printf("[");
        printf(" block num: %d next block %d ", i, dbs[i].next_block_num);
        if(dbs[i].next_block_num != -1)
        {
            printf("USED BLOCK");
        }
        printf("] ");
        if(i % 4 == 0 && i!=0)
            printf("\n\n");
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

char *read_data_test(int filenum, int pos)
{
    // calculate witch block
    int relative_block = pos / BLOCKSIZE;
    // find the block number
    int bn = get_block_num(filenum, relative_block);
    // calculate the offset in the block
    int offset = pos % BLOCKSIZE;
    // read the data
    char *res = malloc(sizeof(char) * 1024);
    strcpy(res, &(dbs[bn].data[offset]));
}


