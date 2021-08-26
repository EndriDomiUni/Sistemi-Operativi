/*
costruire un programma in cui il main crea infiniti thread, 
a ciascuno dei quali passa un indice crescente, senza fare mai la pthread_join.
Ciascun thread stampa l'indice passatogli e termina se stesso
*/
#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

/* Var per sincronissazione */
int num_th_in_scrittura = 0;  /* indica il numero di lettori 
che vuole incrementare result*/

/* VAR GLOBALI */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_myTurm = PTHREAD_COND_INITIALIZER;
int result = 0; /* da aumentare ogni volta */

void *increment(void *arg) {

    
    char Plabel[128]; /* thread name */
    intptr_t index;
    

    index = (intptr_t) arg;
    sprintf(Plabel, "th%" PRIiPTR " ", index);

    printf("thread %s creato\n", Plabel);
    fflush(stdout);

    while (1) {
        
        /* ottengo mutua esclusione */
        DBGpthread_mutex_lock(&mutex, Plabel);
        
        
        
        while (num_th_in_scrittura > 0) {
            printf("thread %s attende fine degli altri\n", Plabel);
            fflush(stdout);
            DBGpthread_cond_wait(&cond_myTurm, &mutex, Plabel);
        }

      
        if (num_th_in_scrittura <= 0) {
           
            printf("thread %s inizia per primo", Plabel);
            fflush(stdout);
        }
        

        /* aggiorno result */
        result++;

        /* aggiorno il numero di th che voglio incrementare */
         num_th_in_scrittura++; 

        /* stampo a video */
       
        printf("thread %s inizia scrittura\n", Plabel);
        fflush(stdout);

        printf("thread %s aggiorna result a: %d\n", Plabel, result);
        fflush(stdout);

        /* rilascio mutua esclusione */
        DBGpthread_mutex_unlock(&mutex, Plabel);
 
        /* SVEGLIO GLI ALTRI THREAD  */
        DBGpthread_cond_signal(&cond_myTurm, Plabel);
        DBGpthread_mutex_lock(&mutex, Plabel);
        

        printf("thread %s sveglia thread in attesa\n", Plabel);
        fflush(stdout);
        DBGpthread_mutex_unlock(&mutex, Plabel);

       
        
        
/*
while (1)
        {
        DBGpthread_mutex_lock(&mutex, Plabel);
        result++;

        printf("Attuale = %d\n", result);
        fflush(stdout);
        DBGpthread_mutex_unlock(&mutex, Plabel);
         }
*/
        
    }
    pthread_exit(NULL);
}

int main(int argc, char* *argv) {

    pthread_t   th;
    int rc;
    intptr_t i;
    

    rc = pthread_mutex_init(&mutex, NULL);
    if ( rc ) PrintERROR_andExit(rc, "pthread_mutex_init failed");
    rc = pthread_cond_init(&cond_myTurm, NULL);
    if ( rc ) PrintERROR_andExit(rc, "pthread_cond_init failed");

    /* creo 10 thread */
    DBGpthread_mutex_lock(&mutex, "main");

    for (i = 0; i < 10; i++) {
        rc = pthread_create(&th, NULL, increment, (void*) i);
        if ( rc ) PrintERROR_andExit(rc, "pthread_create failed");
    }

    pthread_exit(NULL);
    return 0;
}
