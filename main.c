#include "file.h"
#include "Mylibc.h"

int main(int argc, char const *argv[])
{
    mymkfs(10000);

    // mymount(NULL,"fs_data",NULL,0,NULL);    
    // // mymount("fs_data",NULL,NULL,0,NULL);

    // int y = myopen("folder/afik", 1);
    
    // mywrite(y,"AFIK IA HOMO!!!\n",15);

    // mylseek(y, 0, 0);    

    // char buf [100];
    // myread(y, buf, 50);
    // printf("%s\n",buf);

    // myclose(y);

    // print_fs();

    /************************AFIK***************************/

    mymount("fs_data",NULL,NULL,0,NULL);
    
    char res1 [100];

    char* res2 =  "WELLCOM TO AFIK AND ARIEL UFS!!!";
    
    myFILE *ariel =  myfopen("folder/ariel", "w");

    myfwrite(res2,31,1,ariel);

    myfseek(ariel, 0 , SEEK_SET);

    myfclose(ariel);

    myFILE *afik =  myfopen("folder/ariel", "r");

    myfread(res1, 30, 1 , afik);
    printf("%s\n",res1);

    // myfclose(ariel);

    printf("done!\n");
    return 0;
}