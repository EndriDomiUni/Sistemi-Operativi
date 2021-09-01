#!/bin/bash

# uso questo script per generare nomi diversi di file

FILE=`ls`

for nome1 in ${FILE} ; do
    for nome2 in ${FILE} ; do
        for nome3 in ${FILE} ; do
            echo "(${nome1}, ${nome2}, ${nome3})"
        done
    done 
done

