#!/bin/bash

IFS=$',\n'

while read PAROLA NUMERO ; do
	echo "${NUMERO},${PAROLA},${NUMERO}"
done < input2.txt | sort > temp.txt

while read NUMERO PAROLA ALTRONUMERO ; do
	echo "${PAROLA},${ALTRONUMERO}"
done < temp.txt
rm temp.txt

