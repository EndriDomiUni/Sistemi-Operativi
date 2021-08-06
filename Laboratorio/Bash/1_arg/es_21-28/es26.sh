#!/bin/bash

# Scrivere uno script che esegue i seguenti compiti:
# Creare una directory BUTTAMI, dentro questa creare dei file che si chiamano
# * ** *** ;;
# Fare un listing di questi file e, per ciascun o dei file nella directory 
# aggiungere un nuovo file con stesso nome con in più l'estensione .txt.


mkdir BUTTAMI ; touch BUTTAMI/*.txt ; touch BUTTAMI/"**".txt; touch BUTTAMI/"***".txt ;

cp -R /Users/endridomi/Desktop /Users/endridomi/Documents/GitHub/Sistemi-Operativi/Laboratorio/Bash/1_arg/es_21-28/BUTTAMI/ ;

find ./BUTTAMI -type d 

rm -rf BUTTAMI/Desktop
