#!/bin/bash

for name in `/Users/endridomi/Desktop`; do 
    if [[   -d /Users/endridomi/Desktop/${name} &&  -r /Users/endridomi/Desktop/${name}]] ;
        then echo ${name}
    fi 
done 

# non funziona