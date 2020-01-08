#include <stdio.h>
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include "saferun.h"
#include <unistd.h>
#include <limits.h>
#define str(s) #s

void* libhandle = 0;

void (*printjunk_ptr)(int) = 0;
void (*printjunk2_ptr)(int, char*, int) = 0;
float (*printjunkf_ptr)(int, float) = 0;
char workdir[PATH_MAX + 1] = {0};

void init(const char* wd)
{
    snprintf(workdir, PATH_MAX, "%s", wd);
	printf("workdir %s\n",wd);
}

void unloadLibrary()
{
    if(libhandle != 0)
    {
        printf("closing lib\n");
        dlclose(libhandle);
        libhandle = 0;
    }
}

void loadLibrary()
{
    unloadLibrary();

    if(workdir[0] == 0)
    {
        return;
    }

    printf("loading lib from %s\n", workdir);
    chdir(workdir);
    libhandle = dlopen("./libfcloth.so", RTLD_LAZY);
    printjunk_ptr = dlsym(libhandle, "printjunk");
    printjunk2_ptr = dlsym(libhandle, "printjunk2");
    printjunkf_ptr = dlsym(libhandle, "printjunkf");
}

sigjmp_buf before = {0};


static void sighandler(int sig, siginfo_t* info, void* ucontext)
{
    fprintf(stderr, "\e[1;31mops! something went wrong\e[0m\n");
    longjmp(before, 1);
}

#define LOADCHECK(FUNCPTR)  \
    if(libhandle == 0) \
    { \
		loadLibrary(); \
    } \
    if(FUNCPTR != 0) \


void printjunk(int num)
{
    LOADCHECK(printjunk_ptr)
    {
        SAFERUN((*printjunk_ptr)(num);)
    }
}

void printjunk2(int num, char* f, int i)
{
    LOADCHECK(printjunk2_ptr)
    {
        SAFERUN((*printjunk2_ptr)(num, f, i);)
    }
}
void printjunkf(int num, float f)
{
    float i = 0;
    LOADCHECK(printjunkf_ptr)
    {
        SAFERUN(i = (*printjunkf_ptr)(num, f);)
    }
}

int main(int argc, char* argv[])
{
    while(1)
    {
        loadLibrary();
        printjunk(0);
    }

    printjunk2(0, 0, 0);
    printjunkf(0, 4.2);
    return 0;
}
