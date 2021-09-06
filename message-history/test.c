#include <stdio.h>
#include <stdlib.h>

struct video
{
    char *link;
    int *time; 
};

int main()
{
    struct video vid;

    int len = 5; // примерна дължина
    vid.link = (char*) malloc(len*sizeof(char));
    vid.time = (int*) malloc(len*sizeof(int));

    return 0;
}