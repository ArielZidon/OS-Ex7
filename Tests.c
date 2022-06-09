#include "file.h"
#include "Mylibc.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);
    mymount(NULL,"fs_data",NULL,0,NULL); 

    printf("\033[0;36m****************************TEST PART 1**********************************\n\n");

    int y = myopen("folder/afik", 1);
    mywrite(y,"THE READ AND WRITE IS WORK!!!\n",30);
    mylseek(y, 0, 0);    
    char buf [100];
    myread(y, buf, 50);
    printf("%s\n",buf);
    myclose(y);

    bzero(buf,sizeof(buf));

    int x = myopen("folder/afik", 1);
    mywrite(x,"THE READ AND WRITE IS WORK IN CASE NUMBER 1!!!\n",50);
    mylseek(x, 0, 0);    
    myread(x, buf, 50);
    printf("%s\n",buf);
    myclose(x);

    bzero(buf,sizeof(buf));

    int z = myopen("folder/afik", 1);
    mywrite(z,"THE READ AND WRITE IS WORK IN CASE NUMBER 2!!!\n",50);
    mylseek(z, 0, 0);    
    myread(z, buf, 50);
    printf("%s\n",buf);
    myclose(z);

    bzero(buf,sizeof(buf));

    int d = myopen("folder/afik", 1);
    mywrite(d,"THE READ AND WRITE IS WORK IN CASE NUMBER 3!!!\n",50);
    mylseek(d, 0, 0);    
    myread(d, buf, 50);
    printf("%s\n",buf);
    myclose(d);

    
    printf("\033[0;36m****************************TEST PART 2**********************************\n\n");
    for (int i = 0; i < 3; i++)
    {
        printf("TEST PASS: Was able to open the disk\n");
        myFILE *myfd = myfopen("folder/file", "w");
        if (myfprintf(myfd, "%c%d", (char)i + 'a', i) == 2)
        {
            printf("TEST PASS: Was able to write a char and int\n");
        }
        myfseek(myfd, 0, SEEK_SET);
        char c = '0';
        int n = 0;
        myfclose(myfd);
        myfd = myfopen("folder/file", "r");
        if (myfscanf(myfd, "%c%d", &c, &n) == 2)
        {
            printf("TEST PASS: Was able to read:\n\t char- %c and int- %d \n", c, n);
        }
        printf("\n");
        myfclose(myfd);
        printf("TEST PASS: Was able to close the file");

        myfd = myfopen("folder/file", "w");
        if (myfprintf(myfd, "%d", 10-i) == 1)
        {
            printf("TEST PASS: Was able to write a big number\n");
        }
        printf("\n");
        myfclose(myfd);
        myfd = myfopen("folder/file", "r");
        c = '0';
        n = 0;
        int f = 0;
        if (myfscanf(myfd, "%c%d%f", &c, &n, &f) == 3)
        {
            printf("TEST PASS: Was able to read:\n\t char- %c and int- %d and big number- %d\n", c, n, f);
        }
        printf("\n");
        myfclose(myfd);
        printf("TEST PASS: Was able to close the file\n");
        printf("\n");
    }
    printf("\n");

    printf("\033[0;36m****************************************FILE SYSTEM****************************************\n\n\n");
    print_fs();
    printf("\n\n");
    return 0;
}