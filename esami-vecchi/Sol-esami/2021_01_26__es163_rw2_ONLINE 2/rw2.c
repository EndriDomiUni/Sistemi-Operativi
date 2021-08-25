/* file:  rw2.c */

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

#define NUMLETTORIA 3
#define NUMLETTORIB 3
#define NUMSCRITTORI 2
/* tempistiche */
#define MINSECGIROLETTORI 2
#define MAXSECGIROLETTORI 12
#define MINSECGIROSCRITTORI 0
#define MAXSECGIROSCRITTORI 4
#define SECLETTURA 2
#define SECSCRITTURA 3

/* variabili per la sincronizzazione */
pthread_mutex_t  mutexVarLettori;
pthread_mutex_t  mutexBuffer;
int numLettoriInLettura=0;  /* numero lettori che stanno leggendo */

/* VARIABILE PER FAR ATTENDERE I LETTORI DELL'ALTRO TIPO */
pthread_cond_t	condAttesaLettoriAltroTipo;

/* VARIABILE PER SAPERE CHE TIPO DI LETTORE STA LEGGENDO
   questo valore puo' essere 'A' o 'B' ma deve essere considerato
   solo quando numLettoriInLettura>0  altrimenti non ha senso.  */
char tipoLettoreInLettura='0'; /* 'A' 'B' - valido solo se numLettoriInLettura>0 */

/* variabile globale letta e scritta */
uint64_t valGlobale=0;

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

void *Scrittore (void *arg) 
{ 
	char Slabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Slabel,"S%" PRIiPTR "", indice);

	while(1) {
		/* prendo la mutua esclusione */
		DBGpthread_mutex_lock(&mutexBuffer,Slabel); 
		/* scrivo */
		valGlobale++;

		printf("Scrittore %s inizia scrittura \n", Slabel ); 
		fflush(stdout);

		/* scrittore impiega tempo a scrivere */
		attendi(SECSCRITTURA,SECSCRITTURA);

		printf("Scrittore %s ha scritto %lu \n", Slabel, valGlobale ); 
		fflush(stdout);

		/* rilascio mutua esclusione */
		DBGpthread_mutex_unlock(&mutexBuffer,Slabel); 

		/* SVEGLIO LETTORI CHE DEVONO ESSERE SVEGLIATI */
		/* INIZIO PRIMA PARTE DA COMPLETARE PER TIPI LETTORI DA QUI */
		DBGpthread_mutex_lock(&mutexVarLettori,Slabel); 
		DBGpthread_cond_broadcast(&condAttesaLettoriAltroTipo,Slabel);
		printf("Scrittore %s sveglia lettori in attesa  \n", Slabel ); 
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutexVarLettori,Slabel); 
		/* FINE PRIMA PARTE DA COMPLETARE PER TIPI LETTORI FINO A QUI */

		/* scrittore fa un giro non in mutua esclusione */
		attendi(MINSECGIROSCRITTORI,MAXSECGIROSCRITTORI);
	}
	pthread_exit(NULL); 
} 


void *Lettore (char mioTipoLettore, intptr_t indice) 
{ 
	char Llabel[128];

	sprintf(Llabel,"L%c%" PRIiPTR "", mioTipoLettore, indice);

	while(1) {
		/* prendo la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_lock(&mutexVarLettori,Llabel); 

		/* controllo se stanno gia' leggendo dei lettori dell'altro tipo */
		/*  INIZIO SECONDA PARTE DA COMPLETARE PER TIPI LETTORI DA QUI */
		while ( numLettoriInLettura>0 && tipoLettoreInLettura!=mioTipoLettore) {
			/* stanno leggendo lettori di altro tipo, aspetto */
			printf("Lettore %s attende fine altri lettori\n", Llabel);
			fflush(stdout);
			DBGpthread_cond_wait(&condAttesaLettoriAltroTipo,&mutexVarLettori,Llabel);
		}
		/*  FINE SECONDA PARTE DA COMPLETARE PER TIPI LETTORI FINO A QUI */

		/* se non ci sono altri lettori gia' a leggere dal buffer
		   significa che nessun altro lettore ha gia' preso la mutua esclusione
		   per il gruppo dei lettori, quindi
		   devo prendere io la mutua esclusione per tutto il gruppo dei lettori
		*/
		if ( numLettoriInLettura <= 0 ) {
			/* attendo di poter leggere, sono il primo */
			printf("Lettore %s sarebbe il primo a leggere\n", Llabel);
			fflush(stdout);
			DBGpthread_mutex_lock(&mutexBuffer,Llabel); 

			/* sono il primo a leggere, fisso il tipo di lettori */
			/*  INIZIO TERZA PARTE DA COMPLETARE PER TIPI LETTORI DA QUI */
			tipoLettoreInLettura=mioTipoLettore;
			/*  FINE TERZA PARTE DA COMPLETARE PER TIPI LETTORI FINO A QUI */
		}
		/* ora vado in lettura anche io */
		numLettoriInLettura++;
		/* ma permetto ad altri lettori di vedere come stanno le cose */
		/* rilascio la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_unlock(&mutexVarLettori,Llabel); 

		/* leggo quel che c'e' nel buffer */
		printf("Lettore %s inizia lettura \n", Llabel ); 
		fflush(stdout);
		/* il lettore impiega un po' di tempo a leggere */
		attendi(SECLETTURA,SECLETTURA);
		/* ho letto */
		printf("Lettore %s ha letto %lu \n", Llabel, valGlobale ); 
		fflush(stdout);

		/* riprendo la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_lock(&mutexVarLettori,Llabel); 
		/* io non leggo piu' */
		numLettoriInLettura--;
		/* se sono l'ultimo lettore che era in lettura,
		   devo rilasciare la mutua esclusione per il gruppo dei lettori
		*/
		if ( numLettoriInLettura <= 0 ) {
			/* rilascio mutua esclusione sul buffer */
			DBGpthread_mutex_unlock(&mutexBuffer,Llabel); 
			/* ho ancora mutua esclusione sulla variabile dei lettori */

			/* non ci sono piu' lettori del mio stesso tipo in lettura,
			   devo consentire ai lettori dell'altro tipo di leggere */
			/* INIZIO QUARTA PARTE DA COMPLETARE PER TIPI LETTORI DA QUI */

			/* resetto il tipo di lettori in lettura */
			tipoLettoreInLettura='0';
			/* solo dopo sveglio i lettori dell'altro tipo */
			DBGpthread_cond_broadcast(&condAttesaLettoriAltroTipo,Llabel);
			printf("Ultimo lettore %s sveglia lettori altro tipo\n", Llabel); 
			fflush(stdout);
			/*  FINE QUARTA PARTE DA COMPLETARE PER TIPI LETTORI FINO A QUI */
		}
		/* rilascio la mutua esclusione sulla variabile dei lettori */
		DBGpthread_mutex_unlock(&mutexVarLettori,Llabel); 

		/* fuori da mutua esclusione faccio un giro */
		printf("Lettore %s fa giro\n", Llabel ); 
		fflush(stdout);
		attendi(MINSECGIROLETTORI,MAXSECGIROLETTORI);
	}
	return(NULL); 
}

void *LettoreA (void *arg) 
{ 
	void *ptr;
	intptr_t indice;
	indice=(intptr_t)arg;
	ptr=Lettore('A',indice); 
	pthread_exit(ptr); 
}

void *LettoreB (void *arg) 
{ 
	void *ptr;
	intptr_t indice;
	indice=(intptr_t)arg;
	ptr=Lettore('B',indice); 
	pthread_exit(ptr); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_mutex_init(&mutexBuffer, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_mutex_init(&mutexVarLettori, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_cond_init(&condAttesaLettoriAltroTipo, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");


	/* creo i thread */
	for(i=0; i<NUMLETTORIA; i++) {
		rc=pthread_create(&th, NULL, LettoreA, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0; i<NUMLETTORIB; i++) {
		rc=pthread_create(&th, NULL, LettoreB, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0; i<NUMSCRITTORI; i++) {
		rc=pthread_create(&th, NULL, Scrittore, (void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	pthread_exit(NULL);
	return(0); 
} 
  
  
  
