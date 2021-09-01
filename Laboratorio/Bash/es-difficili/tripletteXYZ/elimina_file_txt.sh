#!/bin/bash

NOMIFILE=`ls`

for nome in ${NOMIFILE} ; do
    echo "${nome} sta per essere rimosso"
        rm *.txt
    echo "rimosso file"
done