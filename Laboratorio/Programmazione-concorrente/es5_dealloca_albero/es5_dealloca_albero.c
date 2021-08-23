#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct s_NODO{

    int key;
    double x;
    struct s_NODO *destra;
    struct s_NODO *sinistra;

} NODO;

