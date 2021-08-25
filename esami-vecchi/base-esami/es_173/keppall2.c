/* file:  keppall2.c */

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

#define NUMKEPPALLINIZIALI 5
#define MINNEWKEPPALL 0
#define MAXNEWKEPPALL 2
#define MINSECVITA 1
#define MAXSECVITA 4
/* tempistiche */
/* dati da proteggere */
int morto=0;	/* Vale se 1 c'e' un thread morto per cui fare la join
		   ed in tal caso nella variabile th_morto c'e' l' id del pthread.
		   Vale 0 altrimenti.  */
pthread_t th_morto;    /* id ultimo pthread morto/moribondo */
intptr_t indice_morto; /* Usato solo per l'output: indice ultimo pthread morto o moribondo */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex=PTHREAD_MUTEX_INITIALIZER;
/* qui potete aggiungere altre vostre variabili globali, se vi servono */
/* COMPLETARE PRIMA PARTE A PARTIRE DA QUI */
pthread_cond_t condCoda;
pthread_cond_t condMain;
int morente;


/* COMPLETARE PRIMA PARTE FINO A QUI */

void attendi( int secmin, int secmax) {
        int secrandom=0;
	static int inizializzato=0;
	if( inizializzato==0 ) {
		srandom( time(NULL) );
		inizializzato=1;
	}
        if( secmin > secmax ) return;
        else if ( secmin == secmax )
                secrandom = secmin;
        else
                secrandom = secmin + ( random()%(secmax-secmin+1) );
        do {
                /* printf("attendi %i\n", secrandom);fflush(stdout); */
                secrandom=sleep(secrandom);
                if( secrandom>0 )
                        { printf("sleep interrupted - continue\n"); fflush(stdout); }
        } while( secrandom>0 );
        return;
}

void *Keppall2 (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"K%" PRIiPTR "", indice);

	printf("keppall %s nato\n", Plabel);
	fflush(stdout);
	/* keppall vive */
	attendi( MINSECVITA,MAXSECVITA );
	/* keppall cerca di morire */

	DBGpthread_mutex_lock(&mutex,Plabel); 
	/* COMPLETARE SECONDA PARTE A PARTIRE DA QUI */

	/* aspetto se sta gia' morendo qualcuno, cioe' aspetto se
	   c'e' gia' qualcuno in attesa che gli facciano la join */

	while(morente != indice) {
		DBGpthread_cond_wait(&condCoda, &mutex, Plabel);
	}

	morente = indice;

	/* COMPLETARE SECONDA PARTE FINO A QUI */
	printf("keppall %s ora puo morire\n", Plabel);
	fflush(stdout);
	/* finalmente ora sta a me di morire */
	/* dico che ora e' il mio turno di morire */
	morto=1;
	th_morto=pthread_self();
	indice_morto=indice;
	/* COMPLETARE TERZA PARTE A PARTIRE DA QUI */


	/* avviso il main che sono morto */
	/* sveglio il main perche' faccia la join per me */

	DBGpthread_cond_signal(&condMain, Plabel);
	DBGpthread_cond_signal(&condCoda, Plabel);
	pthread_exit(morto);



	/* COMPLETARE TERZA PARTE FINO A QUI */
	DBGpthread_mutex_unlock(&mutex,Plabel); 
	/* muoio */
	pthread_exit(NULL);
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc, numth;
	intptr_t i, i2;
	void *ptr;

	/*Mie condizioni*/
	rc=DBGpthread_mutex_init(&condCoda,NULL,Plabel);
	if(rc) PrintERROR_andExit(rc,"Pthread_cond_init failed");
	rc=DBGpthread_mutex_init(&condMain,NULL,Plabel);
	if(rc) PrintERROR_andExit(rc,"Pthread_cond_init failed");

	/* inizializzazione generazione numeri random */
	srandom( time(NULL) );
	/* all'inizio nessun pthread morto */
	morto=0;

	/* creo i primi thread keppall2 iniziali */
	DBGpthread_mutex_lock(&mutex,"main"); 
	for(i=0; i<NUMKEPPALLINIZIALI; i++) {
		rc=pthread_create(&th, NULL, Keppall2, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	DBGpthread_mutex_unlock(&mutex,"main"); 
	while(1) {
		/* attendo che un keppall abbia messo il suo thread identifier
		   nella variabile th_morto e che abbia messo morto a 1 */

		DBGpthread_mutex_lock(&mutex,"main"); 
		/* COMPLETARE TERZA PARTE A PARTIRE DA QUI */


		DBGpthread_cond_wait(&condMain, &mutex, Plabel);

		/* Ora sono stato svegliato da un thread che vuole morire e 
		   che ha gia' messo il proprio id nella var th_morto
		   ed ora e' morto o sta per morire.
		   Attendo che il thread sia morto veramente. */

		/* Faccio la join per quel thread */
		pthread_join(&th, indice_morto);


		/* ora so per certo che il thread e' morto */
		printf("main join thread  %" PRIiPTR "\n", indice_morto);
		fflush(stdout);

		/* avviso che ora puo' morire il prossimo keppall */

		DBGpthread_cond_signal(&condCoda,Plabel);
		morente = -1;


		/* COMPLETARE TERZA PARTE FINO A QUI */
		DBGpthread_mutex_unlock(&mutex,"main"); 
		/* creo un po' di thread nuovi */
		numth=MINNEWKEPPALL+(random()%(MAXNEWKEPPALL+1-MINNEWKEPPALL));
		printf("creo %i thread\n", numth);
		fflush(stdout);
		for(i2=0; i2<numth; i2++,i++) {
			rc=pthread_create(&th, NULL, Keppall2, (void*)i); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
	}
	/* qui in realta' non ci arrivo mai */
	pthread_exit(NULL);
	return(0); 
} 
  
