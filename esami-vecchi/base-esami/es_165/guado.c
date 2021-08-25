/* file:  guado.c 
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

#define NUMCANGURI 4
/* tempistiche */
#define MINSECPASSO 2
#define MAXSECPASSO 4
#define MINSECGIRO  3
#define MAXSECGIRO 10
#define OCCUPATO 1
#define LIBERO   2

#define NUMMASSI 5
#define NUMPASSIPERATTRAVERSARE (NUMMASSI+1)
/* dati da proteggere */
int Masso[NUMMASSI]; /* stato occupato/libero dei massi */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t	cond[NUMMASSI];

/* qui potete aggiungere altre vostre variabili globala, se vi servono */


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

void *Canguro (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"C%" PRIiPTR "", indice);

	while(1) {
		/* arrivo al guado */
		int passifatti=0;
			/* indica la posizione in cui si trova il canguro
			   0==sponda iniziale, 1==primo masso ..
			   e contemporaneamente indica l'indice del prossimo
			   masso in cui si vuole arrivare
			   0==primo masso, 1==secondo masso, NUMMASSI==sponda finale */
		/* cerco di fare uno dopo l'altro 
		   gli (NUMMASSI+1) passi per attraversare */
		for( passifatti=0; passifatti<(NUMMASSI+1); passifatti++ ) {
			/* un salto alla volta */
			/* controllo se il prossimo masso e' libero */
			/* ma solo se il prossimo passo non e' la sponda finale */
			if(passifatti<NUMMASSI) {
				/* il prossimo salto mi spostera' su un masso,
				   non sulla sponda finale */

				DBGpthread_mutex_lock(&mutex,Plabel); 
				printf("canguro %s verifica se masso %i e\' libero \n", Plabel, passifatti);
				fflush(stdout);
				/* PRIMA PARTE DA COMPLETARE A PARTIRE DA QUI */

				/* aspetto che il masso su cui voglio saltare sia libero */
				
				if (Masso[passifatti] == OCCUPATO) {
					DBGpthread_cond_wait(&cond[passifatti], &mutex, Plabel);
					printf("canguro %s aspetta che il masso sia libero\n", Plabel);
					fflush(stdout);
				}

				/* ora che il prossimo masso e' libero 
				   lo occupo io preliminarmente */
				Masso[passifatti]=OCCUPATO;

				

				/* FINE PRIMA PARTE DA COMPLETARE FINO A QUI */

				/* Ora libero il masso precedente e avviso chi e' dietro
				   che il masso e' libero
				   MA SOLO SE ho gia' fatto almeno un passo e quindi
				   il masso precedente NON e' la sponda iniziale
				*/
				if( passifatti>0 ) {
					/* SECONDA PARTE DA COMPLETARE A PARTIRE DA QUI */
					Masso[passifatti-1]=LIBERO;
					DBGpthread_cond_broadcast(&cond[passifatti-1], Plabel);


					/* FINE SECONDA PARTE DA COMPLETARE FINO A QUI */
					/* inizio salto da masso verso prossimo masso */
					printf("canguro %s inizia salto da masso %i\n", Plabel,passifatti-1);
					fflush(stdout);
				} else {
					/* mi sposto da sponda iniziale a primo masso */
					printf("canguro %s inizia salto  da sponda iniziale\n", Plabel);
					fflush(stdout);
				}
				DBGpthread_mutex_unlock(&mutex,Plabel); 
				attendi( MINSECPASSO,MAXSECPASSO );
				printf("canguro %s atterra su masso %i\n", Plabel,passifatti);
				fflush(stdout);
				/* atterrato sul prossimo masso */
			} 
			else 	/* passifatti==NUMMASSI */
				/* prossimo passo atterra su sponda di arrivo */
			{
				DBGpthread_mutex_lock(&mutex,Plabel); 
				/* non devo occupare il prossimo masso, 
				   perche' e' la sponda finale, */

				/* TERZA PARTE DA COMPLETARE A PARTIRE DA QUI */

				/* libero il masso precedente */
				Masso[passifatti-1]=LIBERO;

				/* avviso che il masso precedente e' libero */
				DBGpthread_cond_signal(&cond[passifatti-1],Plabel);


				/* FINE TERZA PARTE DA COMPLETARE FINO A QUI */
				/* inizio salto finale */
				printf("canguro %s inizia salto verso sponda finale \n", Plabel);
				fflush(stdout);
				DBGpthread_mutex_unlock(&mutex,Plabel); 
				attendi( MINSECPASSO,MAXSECPASSO );
				/* atterro sulla sponda */
				printf("canguro %s atterra su SPONDA e parte per fare giro\n", Plabel);
				fflush(stdout);
			}
		} /* fine for, fine passo */

		/* faccio giro */
		attendi( MINSECGIRO,MAXSECGIRO );
		printf("canguro %s finisce giro\n", Plabel);
		fflush(stdout);

	} /* fine while */
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	for(i=0; i<NUMMASSI; i++) {
		rc = pthread_cond_init(&cond[i], NULL);
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	}

	/* all'inizio ogni masso e' libero */
	for(i=0; i<NUMMASSI; i++) Masso[i]=LIBERO;

	/* creo i thread */
	for(i=0; i<NUMCANGURI; i++) {
		rc=pthread_create(&th, NULL, Canguro, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
  
  
