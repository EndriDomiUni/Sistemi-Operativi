#!/bin/bash

NOMEVAR="file_"

for (( NUM=1; ${NUM}<=10; NUM=${NUM}+1 )) ; do # qui uso la doppia tonda per il for che è solo una espressione aritmetica
    echo ${NUM}
    touch ${NOMEVAR}${NUM}.txt
    echo "Ho creato : " ${NOMEVAR}${NUM}.txt
done 



