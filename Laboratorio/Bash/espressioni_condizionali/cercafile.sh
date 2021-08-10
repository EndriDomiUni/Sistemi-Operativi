#!/bin/bash

# Scrivere uno script cercafile.sh che, 
# per ciascuna lettera che sta tra c e g cerca i file 
# (o directory) che stanno nella directory /Users/endridomi/Desktop 
# (non nelle sottodirectory) e che hanno quella lettera come 
# secondo carattere del nome del file, e che verificano una delle due 
# seguenti proprietà: la lunghezza del percorso assoluto del file 
# è minore di 18 OPPURE è maggiore di 23. Di questi file stampa in output il percorso assoluto.

for char in {c..g} ; do
    for name in /Users/endridomi/Desktop/?${char}* ; do 
        if [[ ${#name} -lt 18 || ${#name} -gt 23]] ; then 
            echo ${name}
        fi 
    done 
done 
