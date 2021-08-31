/* file:  turisti.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

/* la   #define _POSIX_C_SOURCE 200112L   e' dentro printerror.h */
#ifndef _BSD_SOURCE
#define _BSD_SOURCE     /* per random e srandom */
#endif

/* messo prima perche' contiene define _POSIX_C_SOURCE */
#include "printerror.h"

#include <unistd.h>   /* exit() etc */
#include <stdlib.h>     /* random  srandom */
#include <stdio.h>
#include <string.h>     /* per strerror_r  and  memset */
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h> /*gettimeofday() struct timeval timeval{} for select()*/
#include <time.h> /* timespec{} for pselect() */
#include <limits.h> /* for OPEN_MAX */
#include <errno.h>
#include <assert.h>
#include <stdint.h>     /* uint64_t intptr_t */
#include <inttypes.h>   /* per PRIiPTR */
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "DBGpthread.h"

#define NUMISTRUTTORI 3
#define NUMTURISTI 10

/* dati da proteggere */
int OrdineDato=0;	/* all'inizio non e' stato dato nessun ordine */
int Eseguiti=0;		/* num turisti che hanno eseguito l'ultimo ordine */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t   condSvegliaTuristi,
		 condSvegliaIstruttori;


void attendi( int min, int max) {
	int secrandom=0;
	if( min > max ) return;
	else if ( min == max ) 
		secrandom = min;
	else
		secrandom = min + ( random()%(max-min+1) );
	do {
		/* printf("attendi %i\n", secrandom);fflush(stdout); */
		secrandom=sleep(secrandom);
		if( secrandom>0 ) 
			{ printf("sleep interrupted - continue\n"); fflush(stdout); }
	} while( secrandom>0 );
	return;
}

void *Turista (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Turista%" PRIiPTR "",indice);

	/*  da completare  */

	/* notare che faccio la lock fuori dal loop */
	DBGpthread_mutex_lock(&mutex,Plabel); 

	while ( 1 ) {

		/* il turista attende l'arrivo dell'ordine */
		printf("turista %s attende ordine \n", Plabel);
	        fflush(stdout);
		DBGpthread_cond_wait(&condSvegliaTuristi,&mutex,Plabel);

		/* il turista esegue ... */
		printf("turista %s inizia a eseguire \n", Plabel);
	        fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		/* il turista impiega da 1 a 6 secondi per eseguire */
		attendi( 1, 6 );

		DBGpthread_mutex_lock(&mutex,Plabel); 
		Eseguiti++;
		printf("turista %s ha finito\n", Plabel);
	        fflush(stdout);

		if( Eseguiti==NUMTURISTI ) {
			Eseguiti=0;
			OrdineDato=0;
			DBGpthread_cond_signal(&condSvegliaIstruttori,Plabel); 
		}

		/* NOTARE CHE qui NON FACCIO LA unlock */
	}
	pthread_exit(NULL); 
}


void *Istruttore (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Istruttore%" PRIiPTR "",indice);

	sleep(5); /* l'istruttore attende 5 secondi iniziali */

	while ( 1 ) {
		DBGpthread_mutex_lock(&mutex,Plabel); 
		while ( OrdineDato==1  )
			DBGpthread_cond_wait(&condSvegliaIstruttori,&mutex,Plabel);

		printf("\n\t istruttore %s da' ordine\n\n", Plabel);
	        fflush(stdout);
		OrdineDato=1;
		DBGpthread_cond_broadcast(&condSvegliaTuristi,Plabel); 
		DBGpthread_mutex_unlock(&mutex,Plabel); 
	}
	pthread_exit(NULL); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	uintptr_t i=0;
	int seme;

	seme=time(NULL);
        srandom(seme);

	/* INIZIALIZZATE LE VOSTRE VARIABILI CONDIVISE / fate voi */

	rc = pthread_cond_init(&condSvegliaTuristi, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condSvegliaIstruttori, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	OrdineDato=0;	/* all'inizio non e' stato dato nessun ordine */
	Eseguiti=0;		/* num turisti che hanno eseguito l'ultimo ordine */

	/* CREAZIONE PTHREAD */
	for(i=0;i<NUMTURISTI;i++) {
		rc=pthread_create(&th,NULL,Turista,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0;i<NUMISTRUTTORI;i++) {
		rc=pthread_create(&th,NULL,Istruttore,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	
	pthread_exit(NULL);
	return(0); 
} 
  
  
  
