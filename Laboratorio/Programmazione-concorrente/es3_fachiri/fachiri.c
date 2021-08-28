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
#define NUM_FACHIRI 2

/* stato deposito */
#define LIBERO 0
#define OCCUPATO 1

/* var globali */
pthread_mutex_t mutex;
pthread_cond_t cond_take_sword;
pthread_cond_t cond_release_sword;
int indice_spada = 0;
int StatoDeposito = LIBERO; /* 0=LIBERO, 1=OCCUPATO */

/* prendi spada */
void *TakeSword(void *arg) {

    char Take_label[128];
    intptr_t index;

    index = (intptr_t) arg;
    sprintf(Take_label, "Prende spada%" PRIiPTR " ", index);

    while (1) {
        /* prendo mutua esclusione */
        DBGpthread_mutex_lock(&mutex, Take_label);

        /* controllo se il deposito è occupato */
        while (StatoDeposito == OCCUPATO) {
            printf("Fachiro %s attende deposito libero\n", Take_label);
            fflush(stdout);
            DBGpthread_cond_wait(&cond_take_sword, &mutex, Take_label);
        }

        /* prelevo spada */
        printf("Fachiro %s preleva spada n°: %d \n", Take_label, indice_spada+1);
        indice_spada++;
        printf("Spade rimanenti: %d\n", NUM_SPADE - indice_spada);

        /* rilascio mutua esclusione */
        DBGpthread_mutex_unlock(&mutex, Take_label);

        /* ho deciso io cosi */
        sleep(1);

        /* avviso l'altro fachiro */
        DBGpthread_mutex_lock(&mutex, Take_label);
        printf("Fachiro %s ha finito di prelevare spada \n", Take_label);
        fflush(stdout);

        /* cambio lo stato */
        StatoDeposito = LIBERO;

        DBGpthread_cond_signal(&cond_take_sword, Take_label);
        DBGpthread_mutex_unlock(&mutex, Take_label);

    }

}

/* trafiggiti e rilascia spada */
void *ReleaseSword(void *arg) {

    char Rel_label[128];
    intptr_t index;

    index = (intptr_t) arg;
    sprintf(Rel_label, "lascia spada%" PRIiPTR " ", index);

    while (1) {

        /* prendo mutua esclusione */
        DBGpthread_mutex_lock(&mutex, Rel_label);

        StatoDeposito = OCCUPATO;

        /* finche non ho zero spade */
        while (indice_spada >= 0) {

            indice_spada--;
            printf("Fachiro %s preleva spada\n", Rel_label);
            fflush(stdout);

            printf("Fachiro %s si infligge dolore: Ahia\n", Rel_label);
            fflush(stdout);

            printf("Spade rimanenti: %d\n", indice_spada);
            fflush(stdout);
        }

        /* rilascio mutua esclusione */
        DBGpthread_mutex_unlock(&mutex, Rel_label);
    }
    pthread_exit(NULL);
}


int main() {

    int rc;
    intptr_t i;
    pthread_t th;

    rc = pthread_mutex_init(&mutex, NULL);
    if (rc) PrintERROR_andExit(rc, "pthread_mutex_init failed");

    
    rc = pthread_cond_init(&cond_take_sword, NULL);
    if (rc) PrintERROR_andExit(rc, "pthread_cond_init failed");

    rc = pthread_cond_init(&cond_release_sword, NULL);
    if (rc) PrintERROR_andExit(rc, "pthread_cond_init failed");


    /* creo i thread (2 fachiri) */
    for (i = 0; i < NUM_FACHIRI; i++) {

        /* azione di prendere */
        rc = pthread_create(&th, NULL, TakeSword, (void*) i);
        if (rc) PrintERROR_andExit(rc, "pthread_create failed");

        /* azione deposito */
        rc = pthread_create(&th, NULL, ReleaseSword, (void*) i);
        if (rc) PrintERROR_andExit(rc, "pthread_create failed");
    }
   


    pthread_exit(NULL);
    return 0;
}
