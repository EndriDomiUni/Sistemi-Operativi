/* file:  keppall2.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 
#include <time.h>
#include <sys/time.h>

#include "DBGpthread.h"
#include "printerror.h"

#define NUM_BANCHE 3

#define NUM_THREADS_DEPOSITO 5
#define NUM_THREADS_PRELIEVO 4

/* VAR GLOBAL */
double T[NUM_BANCHE] = {0.0, 0.0, 0.0}; /* amount foreach bank */
int N[NUM_BANCHE] = {0.0, 0.0, 0.0}; /* num of all operation */

/* var per la sincronizzazione */
pthread_mutex_t mutex;

/* deposito */
void* Deposito(void *arg) {

    char DLabel[128];
    intptr_t index_bank;

    index_bank = (intptr_t) arg;
    sprintf(DLabel, "D%" PRIiPTR " ", index_bank);

    while (1) {
        struct timespec ts = {0, 100000000L };

        sleep(1);
        /* ottengo mutua esclusione */
        DBGpthread_mutex_lock(&mutex, DLabel);

        /* faccio deposito */
        T[index_bank] += 10.0;
        N[index_bank] ++;

        printf("Deposito: N %d, T %f \n", N[index_bank], T[index_bank]);
        fflush(stdout);
        nanosleep(&ts, NULL);

        /* rilascio mutua esclusione */
        DBGpthread_mutex_unlock(&mutex, DLabel);
    }
    pthread_exit(NULL);
}

/* prelievo */
void* Prelievo(void *arg) {

    char PLabel[128];
    intptr_t index_bank;

    index_bank = (intptr_t) arg;
    sprintf(PLabel, "P%" PRIiPTR " ", index_bank);

    while (1) {
        struct timespec ts = {0, 100000000L };

        sleep(1);
        /* ottengo mutua esclusione */
        DBGpthread_mutex_lock(&mutex, PLabel);

        /* faccio deposito */
        T[index_bank] -= 9.0;
        N[index_bank] ++;

        printf("Deposito: N %d, T %f \n", N[index_bank], T[index_bank]);
        fflush(stdout);
        nanosleep(&ts, NULL);

        /* rilascio mutua esclusione */
        DBGpthread_mutex_unlock(&mutex, PLabel);
    }
    pthread_exit(NULL);
}

void* BancaDiItalia(void *arg) {
   
   char BDI_Label[128];
   sprintf(BDI_Label, "Banca di Italia");

   while (1) {
       int i; 
       int num = 0;
       double sum = 0.0;

       sleep(1);
       DBGpthread_mutex_lock(&mutex, BDI_Label);

       for (i = 0; i < NUM_BANCHE; i++) {
           num += N[i];
           sum += T[i];
       }

       DBGpthread_mutex_unlock(&mutex, BDI_Label);
       printf("Report Banca d'Italia: Num %d, sum %f \n", num, sum);
       fflush(stdout);

       sleep(30);
   }
   pthread_exit(NULL);
}

int main() {

    pthread_t th;

    int rc;
    intptr_t i, j;

    rc = pthread_mutex_init(&mutex, NULL);
    if (rc) PrintERROR_andExit(rc, "Failed mutex init");

    for (i = 0; i < NUM_BANCHE; i++) {

        for (j = 0; j < NUM_THREADS_DEPOSITO; j++) {
            rc = pthread_create(&th, NULL, Deposito, (void*) i);
            if (rc) PrintERROR_andExit(rc, "Failed pthread create");
        }

        for (j = 0; j < NUM_THREADS_PRELIEVO; j++) {
            rc = pthread_create(&th, NULL, Prelievo, (void*) i);
            if (rc) PrintERROR_andExit(rc, "Failed pthread create");
        }
    }

    return 0;
}

