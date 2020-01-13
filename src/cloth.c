#include <stdio.h>
#include <math.h>
#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
    #define omp_get_num_threads() 1
#endif

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
// TODO: include blenders header file instead

typedef struct KeyBlock
{
    struct KeyBlock* next, *prev;

    /**
     * point in time   (Key->type == KEY_NORMAL) only,
     * for historic reasons this is relative to (Key->ctime / 100),
     * so this value increments by 0.1f per frame.
     */
    float pos;
    /** influence (typically [0 - 1] but can be more), (Key->type == KEY_RELATIVE) only.*/
    float curval;

    /** interpolation type (Key->type == KEY_NORMAL) only. */
    short type;
    char _pad1[2];

    /** relative == 0 means first key is reference, otherwise the index of Key->blocks */
    short relative;
    short flag;

    /** total number if items in the keyblock (compare with mesh/curve verts to check we match) */
    int totelem;
    /** for meshes only, match the unique number with the customdata layer */
    int uid;

    /** array of shape key values, size is (Key->elemsize * KeyBlock->totelem) */
    void* data;
    /** MAX_NAME (unique name, user assigned) */
    char name[64];
    /** MAX_VGROUP_NAME (optional vertex group), array gets allocated into 'weights' when set */
    char vgroup[64];

    /** ranges, for RNA and UI only to clamp 'curval' */
    float slidermin;
    float slidermax;

} KeyBlock;

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
