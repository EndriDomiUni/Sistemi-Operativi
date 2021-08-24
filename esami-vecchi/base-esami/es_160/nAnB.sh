#!/bin/bash

# controllo l'argomento 
if (( $# != 1)) ; then
    echo "manca l'argomento"
    exit 1;
fi

NA=0                 # num di A
NB=0                 # num di B
RESULT=0             # val da restiture: 0 se ci sono char diversi
STRINGA=$1           # metto l'argomento ricevuto in input in una var
LEN_STR=${#STRINGA}  # in questo modo ottengo la lunghezza di una var

N_LETTI=0            # num di char letti


while (( ${N_LETTI} < ${LEN_STR} )) ; do 
    CAR=${STRINGA:${N_LETTI}:1}
    (( N_LETTI=${N_LETTI}+1 ))

    if [[ ${CAR} == "A" ]] ; then
        (( NA=$NA+1 )) ;
    else 
        if [[ ${CAR} == "B" && ${NA} -gt 0 ]] ; then
            NB=1
            break;
        fi 

        echo falso caso 1
        exit 1
    fi
done

# cerco il resto delle B
while (( ${NLETTI} < ${LUNGHEZZASTRINGA} )) ; do # ci sono altri caratteri da leggere
	# prendo il carattere in posizione NLETTI (all'inizio NLETTI 0)
	CAR=${STRINGA:${NLETTI}:1}
	(( NLETTI=${NLETTI}+1 ))
	if [[ ${CAR} == "B" ]] ; then 
		((NB=$NB+1)) ;
	else
		# arrivo qui solo se ho letto un carattere diverso da B
		# quindi termino senza avere verificato la simmetria
		echo falso caso 2
		exit 2
	fi
done

# verifico di avere letto tante B quante A
if [[ $NA -gt 0 && $NA == $NB ]] ; then
	echo vero  N=$NA
	exit 0
else
	echo falso caso 3
	exit 3
fi