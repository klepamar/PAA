#!/bin/bash

for POMER in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1
do
	../a.out -n 25 -N 300 -m $POMER -d 0 -k 1 -W 250 -C 250 2> /dev/null > pomer_$POMER
done
