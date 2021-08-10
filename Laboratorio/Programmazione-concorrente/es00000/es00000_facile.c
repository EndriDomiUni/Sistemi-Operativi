/* es00000_facile.c  */

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10

void *execute(void *p)
{
    sleep(5);  // ciascun thread attende 5 secondi
    print("%f\n", *((double*)p)); // stampa il valore del puntatore
    free(p); // libera la memoria
    pthread_exit(NULL); // termina il thread (non il processo, a meno che non sia l'ultimo)
}



int main(void) {
    pthread_t th; // dichiaro un thread
    int rc, t; // rc -> pthread_create, t -> contatore
    double *p; // dichiaro il puntatore
    srand(time(NULL));

    for (t=0; t<NUM_THREADS; t++) {
        p = malloc(sizeof(double)) // assegno lo spazio necessario per un double

        if (p == NULL) {
            perror("malloc failed: "); // controllo lo spazio
            pthread_exit(NULL);
        }

        *p = (double) (rand()%100000); // genero un numero rnd
        printf("Creating thread %d\n", t); // stampo il numero del thread

        rc = pthread_create(&th, NULL, execute, p); // esegue la funzione
        
        if (rc) {
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1); // per indicare l'errore
        }
    }
    printf("fine main\n");
    fflush(stdout);
    pthread_exit(NULL);
    return(0);
}