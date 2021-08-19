/*
    Un main lancia in esecuzione 4 thread passando a ciascuno 
    di essi una struttura dati chiamata Struttura che è formata da 3 campi:
    - un intero N che dice al pthread quanti altri thread deve creare;
    - una stringa Str di 100 caratteri;
    - un intero Indice con un valore compreso tra 0 ed N-1, 
      diverso per ciascun thread creato da uno stesso thread.

    Ciascun thread, iniziando la sua esecuzione, aspetta 1 secondo, 
    poi legge il contenuto della struttura dati ricevuta, in particolare il valore di N. 
    Sia M il valore contenuto in N.
    Se M>1 allora il thread lancia in esecuzione M-1 thread passando a ciascuno 
    una copia della struttura dati che nel campo N contiene il valore M-1, 
    e nel campo Indice contiene un valore compreso tra 0 ed (M-1)-1, 
    diverso per ciascun thread creato dal nostro thread.

    Ciascun thread deve restituire una struttura dati simile a quella che è stata avuta come argomento. 
    Nel campo stringa Str della struttura deve essere collocato in formato testuale il valore di N ricevuto 
    poi uno spazio e poi il valore di Indice ricevuto.

    Prima di terminare, ciascun thread deve aspettare la terminazione di ciascun thread che lui 
    ha creato e stampare a video la stringa Str ricevuta da ciascuno di quei thread.
*/

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct s {

    unsigned int N; 
    char[100] Str;
    unsigned int index;

} Struttura;

void *func(void *pointer) {

    /* inizializzo gli attributi*/
    Struttura *strc, *strc1;
    int i, rc;

    strc = (Struttura*)pointer;

    /* stampo */
    printf("Inizio thread N %i Indice %i \n ", strc->N, strc->index);
    fflush(stdout);

    /* attendo 1 sec */
    sleep(1); 

    if (strc->N > 1) {
        pthread_t vth[NUM_THREADS];

        for (i = 0; i < p->N-1; i++) {
            strc1 = (Struttura*) malloc(sizeof(Struttura));

            if (strc1 == NULL) {
                perror("malloc failed! ");
                pthread_exit(NULL);
            }

            strc1->N = strc-> N-1;
            strc1->index = i; 
            strcpy(strc1->str, "msg");

            rc = pthread_create(&vth[t], NULL, func, strc1);

            if (rc) {
                printf("Error: return code from pthread_create() is %d\n", rc);
                exit(1);
            }
        }

        for (i = 0; i < strc->N-1; i++) {
            rc = pthread_join(vth[i], (void*)&strc1);

            if (rc) {
                printf("Error: return code from pthread_create() is %d\n", rc);
                exit(1);
            }

            printf("received \"%s\n", strc1->str);
            fflush(stdout);
            free(strc1);
        }
    }

    sprintf(strc->str, "%i %i ", strc->N, strc->index);
    pthread_exit(strc);
}

int main(void) {

    pthread_t vth[NUM_THREADS];
    int res, i; 
    Struttura *strc;
    srand(time(NULL));
    

    strc = malloc(sizeof(Struttura));

    if (strc == NULL) {
        perror("malloc failed");
        exit(1);
    }

    strc->tid = pthread_self();

    res = pthread_create(&tid, NULL, func, (Struttura*) strc);

    if (res) {
        printf("pthread_create() failed: error %i\n", res);
        exit(1);
    }

    fflush(stdout);
    pthread_exit(NULL);

    return 0;
}