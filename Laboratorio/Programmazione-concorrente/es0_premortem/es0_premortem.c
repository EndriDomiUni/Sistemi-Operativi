#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#define _DEFAULT_SOURCE /* usato per usleep */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct s {
    unsigned int index;  /* indice da stampare */
    pthread_t tid; /* identificatore univoco del thread */
} S; 

void *func(void *arg) {
    void *pret; 
    int res; 
    pthread_t tid;

    S *p1; 
    S *p = (S*)arg;

    printf("indice %d\n", p->index); /* stampo l'indice */
    fflush(stdout);
    usleep(1000); 

    p1=malloc(sizeof(S)); /* alloco a p1 lo spazio necessario a contenere una S */

    /* check malloc */
    if (p1 == NULL) {
        perror("malloc failed");
        exit(1);
    }

    p1->index = p1->index+1;  /* come fare i++, però sui puntatori */
    p1->tid = pthread_self(); /* restituisce l’identificatore del thread che la chiama */

    res = pthread_create(&tid, NULL, func, (void*)p1); /* creo il thread */

    /* check create */
    if (res) {
        printf("pthread_create() failed: error %i\n", res);
        exit(1);
    }

    res = pthread_join( p->tid, &pret); /* sincronizzazione */

    /* check join */
    if (res != 0) {
        printf("pthread_create() failed: error %i\n");
        exit(1);
    }

    free(arg); /* dealloco il puntatore "arg" */
    pthread_exit(NULL); /* con NULL non restituisce nulla */
}


int main(void) {

    int res; pthread_t tid;
    S *p1;

    printf("main indice 0\n");
    fflush(stdout);
    usleep(1000);

    p1 = malloc(sizeof(S)); /* alloco spazio */

    /* check malloc */
    if (p1 == NULL) {
        perror("malloc failed");
        exit(1);
    }

    p1->index = 1;
    p1->tid = pthread_self();

    res = pthread_create(&tid, NULL, func, (void*)p1); /* creo thread */

    /* check create */
    if (res) {
        printf("pthread_create() failed: error %i\n", res);
        exit(1);
    }

    printf("fine main\n");
    fflush(stdout);

    pthread_exit(NULL);
    return 0;
}