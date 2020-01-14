#include <stdio.h>
#include <math.h>
#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
    #define omp_get_num_threads() 1
#endif

#include "DNA_key_types.h"

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

typedef struct vert
{
    float x, y, z;
} vert;

void dothewave(struct KeyBlock* keyBase, struct KeyBlock* keyShape, int time)
{
    if(keyBase->totelem != keyShape->totelem)
    {
        return;
    }

    vert* baseData = keyBase->data;
    vert* shapeData = keyShape->data;
    #pragma omp parallel for

    for(int i = 0; i < keyBase->totelem; i++)
    {
        shapeData[i].x = baseData[i].x + sin((float)time * 0.5 + baseData[i].z * 2.5);
        shapeData[i].y = baseData[i].y + cos((float)time * 0.2 + baseData[i].z * 1);
    }
}
