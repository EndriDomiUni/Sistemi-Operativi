/* file:  catapulte.c */

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

#define MYDEBUG    /* per far stampare lo stato delle catapulte */
#define NUMCATAPULTEPERLATO 4
#define MAXINDICELATO 1
#define NUMCATAPULTETOTALE (NUMCATAPULTEPERLATO*(1+MAXINDICELATO))
/* stato catapulte */
#define INFUNZIONE 1
#define DISTRUTTA 0
/* dati da proteggere */
int statocatapulte[NUMCATAPULTETOTALE];
/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_cond_t   condCatapulte[NUMCATAPULTETOTALE];

/* tipo di dato per creazione thread catapulte */
typedef struct param { int indicelato; int indicecatapulta; } Param;
/* attributi creazione pthread */
pthread_attr_t attr;

/* eventuali altre vostre variabili globali,
   ma non vi dovrebbero servire */


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

void *Proiettile (void *arg) 
{ 
	char Plabel[128];
	intptr_t indicecatapultaBersaglio;

	indicecatapultaBersaglio=(intptr_t)arg;
	sprintf(Plabel,"Proiettile che punta a bersaglio %" PRIiPTR "",indicecatapultaBersaglio);

	attendi(3,3); /* il proiettile vola per 3 secondi */

	/* il proietile ha raggiunto il suo bersaglio,
	   se la catapulta bersaglio e' in funzione allora
	   il priettile la distrugge e la avvisa di terminare */
	DBGpthread_mutex_lock(&mutex,Plabel); 
	printf("proiettile arrivato su catapulta %" PRIiPTR "\n", indicecatapultaBersaglio);
	fflush(stdout);
	if( statocatapulte[indicecatapultaBersaglio]==INFUNZIONE ) {
		printf("proiettile colpisce catapulta in funzione %" PRIiPTR "\n", indicecatapultaBersaglio);
	        fflush(stdout);
		statocatapulte[indicecatapultaBersaglio]=DISTRUTTA;
		/* proiettile avvisa thread catapulta distrutta che deve terminare */
		DBGpthread_cond_signal(&condCatapulte[indicecatapultaBersaglio],Plabel); 
	} else {
		printf("proiettile colpisce catapulta gia\' distrutta %" PRIiPTR "\n", indicecatapultaBersaglio);
	        fflush(stdout);
	}
	DBGpthread_mutex_unlock(&mutex,Plabel); 
	pthread_exit(NULL); 
}

int contaCatapulteInFunzioneSuLato(int indicelato) {
	int i, num=0;
	for( i=0; i<NUMCATAPULTEPERLATO; i++ )
		if( statocatapulte[i+(NUMCATAPULTEPERLATO*indicelato)] == INFUNZIONE )
			num++;
	return( num );
}

#ifdef MYDEBUG
void stampastatocatapulte(void) {
	int i;
	printf("STATO LATO 0 INFUNZIONE ");
	for( i=0; i<NUMCATAPULTEPERLATO; i++ )
                if( statocatapulte[i] == INFUNZIONE )
			printf("%i ", i);
	printf("\n");
	fflush(stdout);
	printf("STATO LATO 0 DISTRUTTI ");
	for( i=0; i<NUMCATAPULTEPERLATO; i++ )
                if( statocatapulte[i] != INFUNZIONE )
			printf("%i ", i);
	printf("\n");
	fflush(stdout);
	
	printf("STATO LATO 1 INFUNZIONE ");
	for( i=NUMCATAPULTEPERLATO; i<2*NUMCATAPULTEPERLATO; i++ )
                if( statocatapulte[i] == INFUNZIONE )
			printf("%i ", i);
	printf("\n");
	fflush(stdout);
	printf("STATO LATO 1 DISTRUTTI ");
	for( i=NUMCATAPULTEPERLATO; i<2*NUMCATAPULTEPERLATO; i++ )
                if( statocatapulte[i] != INFUNZIONE )
			printf("%i ", i);
	printf("\n");
	fflush(stdout);

/*
	printf("\nLATO 1\n");
	for( i=NUMCATAPULTEPERLATO; i<2*NUMCATAPULTEPERLATO; i++ )
                if( statocatapulte[i] == INFUNZIONE )
			printf("%i_1 ", i);
		else 
			printf("%i_0 ", i);
	printf("\n");
	fflush(stdout);
*/
}
#endif


void *Catapulta (void *arg) 
{ 
	char Plabel[128];
	int indicelato, indicecatapulta, indicealtrolato;

	indicelato=((Param*)arg)->indicelato;
	indicecatapulta=((Param*)arg)->indicecatapulta;
	sprintf(Plabel,"CatapultaLato%iIndice%i",indicelato,indicecatapulta);
	free(arg);
	arg=NULL;
	indicealtrolato=(indicelato+1)%2;


	while ( 1 ) {

		/* la catapulta attende di essere svegliata.
		   Se quando si sveglia e' in stato DISTRUTTA allora
		      vuol dire che la catapulta deve terminare,
		   Se invece e' in stato INFUNZIONE allora
		      la catapulta deve sparare.
		*/

		/*  PRIMA PARTE da completare  */
		/* catapulta  attende di essere svegliata */
		
		DBGpthread_mutex_lock(&condCatapulte[indicecatapulta], Plabel);
		printf("Catapulta %s attende di essere svegliata\n", Plabel);
		fflush(stdout);
		DBGpthread_cond_wait(&condCatapulte[indicecatapulta], &mutex, Plabel);
		DBGpthread_mutex_unlock(&condCatapulte[indicecatapulta], Plabel);

		printf("Lo stato di %s è: %d\n", Plabel, statocatapulte[indicecatapulta]);

		/*  FINE PRIMA PARTE da completare  */

		/* la catapulta e' stata svegliata */
		if( statocatapulte[indicecatapulta]==DISTRUTTA ) {
			printf("Entro nel if\n");
			int num;
			printf("colpita catapulta %s del lato %i\n", Plabel, indicelato);
			fflush(stdout);
			/* la catapulta deve terminare il suo thread,
			   ma prima la catapulta colpita controlla se sul suo lato
			   ci sono ancora catapulte in funzione>
			   Se non ce ne sono termina tutto il programma,
			   se ce ne sono ancora allora termina solo se stessa.
			   Il controllo deve essere fatto in mutua esclusione.
			*/
			num=contaCatapulteInFunzioneSuLato(indicelato);

			/*  SECONDA PARTE da completare  */
			DBGpthread_mutex_lock(&mutex, Plabel);
			
			if (num > 0) {
				/* ci sono altre catapulte */
				printf("Num catapulte > 0\n");
				fflush(stdout);
				DBGpthread_cond_signal(&condCatapulte[indicecatapulta], Plabel);
				pthread_exit(NULL);
			}
			DBGpthread_mutex_unlock(&mutex, Plabel);


			/*  FINE SECONDA PARTE da completare  */
			if( num==0 ) {
				/* terminazione programma */
				printf("Catapulta %s era l\'ultima del lato %i\n", Plabel, indicelato);
				printf("Il lato %i perde. Terminazione programma\n", indicelato);
				fflush(stdout);
				exit(0);
			}

		} else {
			printf("Entro nel else\n");
			/* statocatapulte[indicecatapulta]==INFUNZIONE] 
			   La catapulta deve lanciare il proiettile.
			*/
			intptr_t indicecatapultaBersaglio;
			pthread_t    th; 
			int  rc;
			
			/*  TERZA PARTE da completare  */

			DBGpthread_mutex_lock(&mutex, Plabel);
			printf("Catapulta %s è pronta a sparare\n", Plabel);
			fflush(stdout);
			DBGpthread_mutex_unlock(&mutex, Plabel);

			/*  FINE TERZA PARTE da completare  */

			/* viene generato casualmente l'indice della catapulta bersaglio
			   tra quelle dal lato opposto alla catapulta che lancia */
			indicecatapultaBersaglio=
				(NUMCATAPULTEPERLATO*indicealtrolato)+
				(random()%NUMCATAPULTEPERLATO);
			printf("Catapulta %s vuole colpire catapulta %" PRIiPTR "\n", Plabel, indicecatapultaBersaglio);
			fflush(stdout);

			/* genero il pthread proiettile passandogli l'indice
			   della catapulta bersaglio che deve colpire */
			rc=pthread_create(&th,&attr,Proiettile,(void*)indicecatapultaBersaglio); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");

			
		} /* fine else */


	} /* fine while */

	pthread_exit(NULL); 
}



int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc, j;
	uintptr_t i=0;
	int seme;

	/* inizializzazione variabili globali */
	seme=time(NULL);
        srandom(seme);
	for(i=0;i<NUMCATAPULTETOTALE;i++) 
		statocatapulte[i]=INFUNZIONE;

	/* INIZIALIZZATE LE VOSTRE VARIABILI CONDIVISE  fate voi */
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	for(i=0;i<NUMCATAPULTETOTALE;i++) {
		rc = pthread_cond_init(&condCatapulte[i], NULL);
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	}

	/* inizializzazione attributi per creazione thread */
	rc=pthread_attr_init(&attr);
	if( rc ) PrintERROR_andExit(rc,"pthread_attr_init failed");
	rc=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if( rc ) PrintERROR_andExit(rc,"pthread_attr_etdet... failed");

	/* CREAZIONE PTHREAD */
	for(j=0;j<=MAXINDICELATO;j++) {
		for(i=0;i<NUMCATAPULTEPERLATO;i++) {
			Param *p;
			p=malloc( sizeof( Param ) );
			if( p==NULL ) { 
				printf("malloc failed\n");fflush(stdout);
				exit(1); 
			}
			p->indicelato=j;
			p->indicecatapulta=i+(NUMCATAPULTEPERLATO*j);
			rc=pthread_create(&th,NULL,Catapulta,(void*)p); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
	}

	/* per semplicita' il main svolge il ruolo di thread trombone */
	while( 1 ) {
		attendi(5,5);
		/* il trombone suona e avvisa tutte le catapulte
		   di entrambi i lati */
		DBGpthread_mutex_lock(&mutex,"mainTrombone"); 

#ifdef MYDEBUG
		stampastatocatapulte();
#endif
		/*  QUARTA PARTE da completare  */
		printf("Trombone sveglia tutte le catapulte\n");
		fflush(stdout);

		DBGpthread_cond_broadcast(&condCatapulte[i], "mainTrombone");



		/*  FINE QUARTA PARTE da completare  */

		DBGpthread_mutex_unlock(&mutex,"mainTrombone"); 
	}
	/* mai raggiunto */
	pthread_exit(NULL);
	return(0); 
} 
  
  
  
