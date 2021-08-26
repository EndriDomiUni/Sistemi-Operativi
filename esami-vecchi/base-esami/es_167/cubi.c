/* file:  cubi.c 
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

#define NUMPOSTI 6
#define NUMDEPOSITATORI 8
#define NUMPRELEVATORI 4
#define NUMCUBIDADEPOSITAREOGNIVOLTA 1
#define NUMCUBIDAPRELEVAREOGNIVOLTA 2
/* tempistiche */
#define MINSECTAGLIO 2
#define MAXSECTAGLIO 5
#define MINSECDEPOSITO 1
#define MAXSECDEPOSITO 3
#define MINSECPRELIEVO 1
#define MAXSECPRELIEVO 3
#define MINSECCAMION 2
#define MAXSECCAMION 5

#define DEPOSITOCUBI 1
#define PRELIEVOCUBI 2

/* per fare un po' piu' di output */
/* #define DOOUTPUT */

/* dati da proteggere */
int NumCubiSulTavolo=0;
/* dice cosa si potrebbe fare sul tavolo, ma occorre verificare il numero di cubi */
int AzionePossibile=DEPOSITOCUBI; 
/* INIZIO PRIMA PARTE DA COMPLETARE A PARTIRE DA QUI */
/* aggiungete altre vostre variabili se vi servono */


/* FINE PRIMA PARTE DA COMPLETARE FINO A QUI */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t	condDeposito;
pthread_cond_t	condPrelievo;




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

void *Depositatori (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"D%" PRIiPTR "", indice);

	while(1) {
		/* arrivo all'iceberg */
#ifdef DOOUTPUT
		printf("depositatore %s comincia a tagliare cubo da iceberg\n", Plabel);
		fflush(stdout);
#endif
		attendi( MINSECTAGLIO , MAXSECTAGLIO );

		DBGpthread_mutex_lock(&mutex,Plabel); 
#ifdef DOOUTPUT
		printf("depositatore %s vorrebbe depositare cubo\n", Plabel);
		fflush(stdout);
#endif
		while( AzionePossibile!=DEPOSITOCUBI || NumCubiSulTavolo==NUMPOSTI ) {
			printf("depositatore %s attende di depositare cubo\n", Plabel);
			fflush(stdout);
			DBGpthread_cond_wait(&condDeposito,&mutex,Plabel);
		}
		/* ora puo' iniziare a depositare il cubo */
		/* INIZIO SECONDA PARTE DA COMPLETARE A PARTIRE DA QUI */

		/* ora puo' iniziare a depositare il cubo */
		/* INIZIO SECONDA PARTE DA COMPLETARE A PARTIRE DA QUI */
		/* il loro spazio sul tavolo i cubi l'hanno preso
		   anche se non sono completamente appoggiati,
		   ma ci metto tempo a depositare, quindi 
		   - dico se c'e' spazio per depositare altri cubi.
		   - non consento di prelevare i cubi
		*/
		 


		/* formalmente i cubi ci sono ma non sono completamente appoggiati,
		   poiche' i depositatori impiegano tempi diversi per appoggiare
		   i cubi sul tavolo.  Il depositatore attuale non sa quanti cubi
		   sono ancora completamente appoggiati e quanti no.
		   Percio' tengo traccia di quanti sono 
		   ancora non completamente appoggiati  
		*/
		

		/* FINE SECONDA PARTE DA COMPLETARE FINO A QUI */
		/* depositatore ha iniziato a depositare cubo sul tavolo */
		printf("depositatore %s inizia depositare cubo %i\n", Plabel,NumCubiSulTavolo);
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutex,Plabel); 
		/* depositatore impiega del tempo a depositare cubo sul tavolo */
		attendi( MINSECDEPOSITO , MAXSECDEPOSITO );
		DBGpthread_mutex_lock(&mutex,Plabel); 
		/* depositatore ora ha appoggiato completamente il cubo sul tavolo */
		/* INIZIO TERZA PARTE DA COMPLETARE A PARTIRE DA QUI */

		printf("depositatore %s finisce di depositare cubo \n", Plabel);
		fflush(stdout);
		/* attenzione, qui altri potrebbero essere stati 
		   piu' veloci di me a completare il deposito */
		
		
			/* tutti i NUMPOSTI cubi sono veramente sul tavolo */
			/* dico ai prelevatori che possono prelevare */
	

		/* FINE TERZA PARTE DA COMPLETARE FINO A QUI */
		DBGpthread_mutex_unlock(&mutex,Plabel); 

	} /* fine while */
}

void *Prelevatori (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"P%" PRIiPTR "", indice);

	while(1) {
		/* arrivo al tavolo */
		DBGpthread_mutex_lock(&mutex,Plabel); 
#ifdef DOOUTPUT
		printf("prelevatore %s vorrebbe prelevare cubo\n", Plabel);
		fflush(stdout);
#endif
		while( AzionePossibile!=PRELIEVOCUBI || NumCubiSulTavolo==0 ) {
			printf("prelevatore %s attende di prelevare cubo\n", Plabel);
			fflush(stdout);
			DBGpthread_cond_wait(&condPrelievo,&mutex,Plabel);
		}
		/* ora puo' iniziare a prelevare il cubo */
		NumCubiSulTavolo-=NUMCUBIDAPRELEVAREOGNIVOLTA; 
		/*	tolgo i cubi in ZERO secondi, niente attesa */
		printf("prelevatore %s ha prelevato cubo \n", Plabel);
		fflush(stdout);
		/* prelevatore ora ha preso il cubo dal tavolo */
		if( NumCubiSulTavolo==0 ) {
			/* tutti i cubi sono stati tolti dal tavolo */
			/* dico ai depositatori che possono depositare */
			AzionePossibile=DEPOSITOCUBI;
			DBGpthread_cond_broadcast(&condDeposito,Plabel);
			printf("prelevatore %s HA SVUOTATO tavolo \n", Plabel);
			fflush(stdout);
		}
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		/* arrivo al camion */
#ifdef DOOUTPUT
		printf("prelevatore %s mette i cubi sul camion\n", Plabel);
		fflush(stdout);
#endif
		attendi( MINSECCAMION , MAXSECCAMION );

	} /* fine while */
}

int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&condDeposito, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condPrelievo, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	/* creo i thread */
	for(i=0; i<NUMDEPOSITATORI; i++) {
		rc=pthread_create(&th, NULL, Depositatori, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0; i<NUMPRELEVATORI; i++) {
		rc=pthread_create(&th, NULL, Prelevatori, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
  
  
