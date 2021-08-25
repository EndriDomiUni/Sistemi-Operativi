#!/bin/bash

IFS=$',\n'
SOMMA=0

while read PRIMO SECONDO TERZO ; do
    ((SOMMA=${SOMMA}+${SECONDO}))
    echo "${PRIMO},${TERZO}"
done < input1.txt

echo ${SOMMA}