#ifndef __SAFERUN_H__
#define __SAFERUN_H__


#define SAFERUN(X) {\
        struct sigaction sa = {0}; \
        sigemptyset(&sa.sa_mask); \
        sa.sa_flags = SA_NODEFER; \
        sa.sa_sigaction = sighandler; \
        sigaction(SIGILL, &sa, 0); \
        sigaction(SIGSEGV, &sa, 0); \
        if(setjmp(before) == 0) \
        { \
            X \
        } \
        else \
        { \
            printf("tried to run: "#X "\n%s:%i\n",__FILE__,__LINE__); \
        } \
        sa.sa_sigaction = 0; \
        sa.sa_handler = SIG_DFL; \
        sigaction(SIGSEGV, &sa, 0); \
    }


#endif //__SAFERUN_H__
