
/*
Esistono 2 fachiri e un vettore di 10 spade.

L'accesso a ciascuna spada è protetto da una variabile mutex per ciascuna spada. 

I 2 fachiri svolgono continuamente il loro esercizio che consiste nel: 
    1) prendere uno dopo l'altra 10 spade. 
    2) Dopo avere preso tutte le spade, trafiggersi con le spade stampando poi a video un grido di dolore.
    3) Rimettere le spade al loro posto. 4) ricominciare da capo.

Entrambi i fachiri cominciano a prendere le spade cominciando dalla spada di indice minore.
Modellare il sistema mediante un thread per ciascun fachiro.

Iniziate l'esercizio scrivendo il Makefile.
*/

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif

#include "printerror.h"
#include "DBGpthread.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>	/* uint64_t intptr_t */
#include <inttypes.h>	/* uint64_t  PRIiPTR */
#include <sys/time.h>	/* gettimeofday()    struct timeval */
#include <time.h>	/* nanosleep()   struct timespec */
#include <pthread.h> 

#define NUM_SPADE 10
#define NUM_FACHIRI 10


pthread_mutex_t mutex[NUM_SPADE];


void *fachiro(void *arg) {

    int rc, i;

    while (1) {

        for (i = 0; i < NUM_SPADE; i++) {
            /* ottengo mutua esclusione per ogni var mutex */
            rc = pthread_mutex_lock(&(mutex[i]));
            if (rc) PrintERROR_andExit(rc, "pthread_mutex_lock failed\n"); 
        }
        sleep(1);

        /* mi infliggo dolore */
        printf("fachiro %" PRIiPTR "si infligge dolore ... AHIA\n", (uintptr_t) arg);
        fflush(stdout);

        for (i = 0; i < NUM_SPADE; i++) {
            /* rilascio mutua esclusione per ogni var mutex */
            rc = pthread_mutex_unlock(&(mutex[i]));
            if (rc) PrintERROR_andExit(rc, "pthread_mutex_unlock failed\n");    
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {

    pthread_t tid;
    int rc, i;
    uintptr_t t;

    /* creo 10 var mutex */
    for (i = 0; i < NUM_SPADE; i++) {
        rc = pthread_mutex_init(&mutex[i], NULL);
        if (rc) PrintERROR_andExit(rc, "pthread_mutex_init failed\n");
    }

    /* creo i fachiri */
    for (t = 0; t < NUM_FACHIRI; t++) {
        printf("Creating thread %" PRIuPTR "\n", t);
        rc = pthread_create(&tid, NULL, fachiro, (void*) t);
        if (rc) PrintERROR_andExit(rc, "pthread_create failed\n");
    }

    pthread_exit(NULL);
    return 0;
}
