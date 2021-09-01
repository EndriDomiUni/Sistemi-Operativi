/* file:  incrocio.c 
*/ 

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif


#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 
#include "printerror.h"
#include "DBGpthread.h"

#define NUMSTRADE 4
#define NUMAUTOCREATEPERVOLTA 7
#define NUMSECS 17

/* dati da proteggere */
int TurnoGlob[NUMSTRADE]; /* assegna numero per turno di primo in fila di UNA STRADA */
int Turno[NUMSTRADE]; /* chi e' di turno per essere primo in coda di una strada */
int AutoInAttesa[NUMSTRADE]; /* quanti sono in fila su una strada */
int AutoInIncrocio=0;
/* variabili per la sincronizzazione */
pthread_mutex_t  mutex; 
pthread_cond_t   cond; 

/* VIC:  INIZIO PRIMA PARTE DA INSERIRE */
/* se proprio vi servono altre variabili globali, aggiungetele .... */


/* VIC:  FINE PRIMA PARTE DA INSERIRE */

typedef struct parametri {
	int indiceauto;   /* distingue univocamente ciascuna auto */
	int indicestrada; /* indica in quale strada si trova l'auto */
} Parametri;

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
 
/* deve essere eseguita detenendo la mutua esclusione */
int precedenza ( int indicestrada ) {
	/* VIC:  INIZIO SECONDA PARTE DA INSERIRE */

	int i;
	/* SOLO PER DEBUGGING, stampo lo stato delle strade */
	#define DODEBUGGING
	#ifdef DODEBUGGING
	printf("in fila nelle strade:   ");
	for( i=0; i<NUMSTRADE; i++ )
		printf(" %i: %i   ", i, AutoInAttesa[i] );
	printf("\n");
	fflush(stdout);
	#endif

	for (i = 0; i < NUMSTRADE; i++) {
		if (AutoInAttesa[i] > 0) {
			return (0); /* devo dare la precedenza a qualcuno */
		}
	}
	return (1); /* ho la precedenza */
	/* VIC:  FINE SECONDA PARTE DA INSERIRE */
} 


void *Auto (void *arg) 
{ 
	char Plabel[128];
	int indiceauto;
	int indicestrada;
	int mioturno=0;

	indiceauto=((Parametri*)arg)->indiceauto;
	indicestrada=((Parametri*)arg)->indicestrada;
	free(arg);
	sprintf(Plabel,"S%i_A%i", indicestrada, indiceauto);

	/* auto prende biglietto per turno di primo in fila di strada indicestrada-esima */	
	DBGpthread_mutex_lock(&mutex,Plabel); 
	mioturno=TurnoGlob[indicestrada];
	TurnoGlob[indicestrada]++;
	printf("auto %s ha turno %i \n", Plabel, mioturno ); 
	fflush(stdout);

	/* auto si mette in file nella sua strada */
	AutoInAttesa[indicestrada]++;
	/* attendo che sia il mio turno di attraversare l'incrocio */
	/* SE UNA O PIU' DELLE SEGUENTI CONDIZIONI E' FALSA ALLORA devo aspettare
		- l'incrocio e' ABBASTANZA libero (AutoInIncrocio<2).
		- sono il primo della mia fila.
		- ho la precedenza rispetto alle auto delle altre strade.
	*/
	while (AutoInIncrocio < 2 ||  ) {
		/* VIC:  INIZIO TERZA PARTE DA INSERIRE */
		/* utilizzare ANCHE la funzione  precedenza */
		
		/* VIC:  FINE TERZA PARTE DA INSERIRE */
		

		/*
		printf("auto %s turno %i non puo' attraversare, aspetta \n", Plabel, mioturno ); 
		fflush(stdout);
		*/
		DBGpthread_cond_wait(&cond,&mutex,Plabel);
	}

	/* auto entra nell'incrocio  */
	AutoInIncrocio++;
	printf("auto %s entra nell'incrocio %i \n", Plabel, mioturno ); 
	fflush(stdout);
	/* auto esce dalla sua fila e passa il turno al secondo della sua fila 
	   che diventa prima e deve essere svegliata per vedere se puo' passare subito
	*/
	AutoInAttesa[indicestrada]--;  /* sono uscito dalla fila */
	/* VIC:  INIZIO QUARTA PARTE DA INSERIRE */
	/* assegno il turno al successivo della mia fila */
	Turno[indicestrada]++;
	/* se incrocio abbastanza libero, sveglio il successivo della mia fila */
	
	DBGpthread_mutex_unlock(&mutex,Plabel); 
	/* VIC:  FINE QUARTA PARTE DA INSERIRE */

	attendi(2,2); /* impiego 2 sec per attraversare l'incrocio */

	/* esco dall'incrocio e lo libero da me, 
	   sveglio tutti quanti per far passare qualcuno
	*/
	/* VIC:  INIZIO QUINTA PARTE DA INSERIRE */
	
	/* VIC:  FINE QUINTA PARTE DA INSERIRE */

	/* auto va via e il pthread termina  */
	pthread_exit(NULL); 
} 

int main (int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	/* variabile per creare thread detached */
	pthread_attr_t attr;
	int  rc, i;
	unsigned int I=0;
	/* variabile per passaggio parametri */
	Parametri *ParametriPtr;

	/* Inizializzazione generatore numeri casuali */
	srandom( time(NULL) );

	/* Inizializzazione variabili */
	for(i=0;i<NUMSTRADE;i++) {
		TurnoGlob[i]=0;
		Turno[i]=0;
		AutoInAttesa[i]=0;
	}

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	while(1) { /* ogni 25 secondi creo 10 automobili */
		for(i=0;i<NUMAUTOCREATEPERVOLTA;i++) {
			ParametriPtr=malloc(sizeof(Parametri));
			if( !ParametriPtr ) { printf("malloc failed\n");exit(1); }
			/* assegno identificatore univoco ad auto, solo per debug */
			ParametriPtr->indiceauto=I;
			I++;
			/* faccio arrivare l'auto da una strada a caso */
			ParametriPtr->indicestrada=random()%NUMSTRADE;
			/* creo il thread detached */
			rc=pthread_create( &th,NULL,Auto,(void*)ParametriPtr); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
		attendi(NUMSECS,NUMSECS);
	}

	pthread_exit(NULL); 

	return(0); 
} 

