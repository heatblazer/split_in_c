#include "split_ver2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* readFileIntoRam(const char* fname)
{
    FILE* fp = fopen(fname, "r");
    if ( !fp )
        return 0;

    long sz = 0;
    //get the size of file and rewind back
    if ( 1 )
    {
        fseek(fp, 0, SEEK_END);
        sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    char* fdata = (char*)malloc(sizeof(char)* sz);
    char* begin = fdata;

    fread((char*)begin, sz, 1, fp);
    begin[sz] = 0;
    fclose(fp);
    return fdata;

}

/****************************************************************/
//Unit testing
//main()
int main(int argc, char** argv)
{
    char* fileData = readFileIntoRam(argv[1]);

    //1st pass with a small bug with the last string
    //fix: fixed the bug with the missing 0 terminator
    int i = split(fileData, ':');
    //test the size
    printf("Size is: [%d]\n", i);

    //test the get
    int j;
    for(j=0; j < getSize(); j++)
    {
        printf("[%d]=[%s]\n", j, getAt(j));
    }



    return 0;
}

/****************************************************************/

