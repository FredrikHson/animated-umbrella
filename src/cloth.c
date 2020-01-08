#include <stdio.h>

void printjunk(int num)
{
    printf("junk %i\n", num);
}
void printjunk2(int num, char* f, int i)
{
    int* j = 0;
    /**j=1;*/
    /*printf("junk2 %i\n", num / 0);*/
    printf("junk %i %s\n", num * 55, f);
}

void printjunkf(int num, float f)
{
    int* j = 0;
    *j = 1;
    printf("junkf %i %f\n", num, f);
}
