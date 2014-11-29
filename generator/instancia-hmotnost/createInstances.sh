#!/bin/bash

for MAX_WEIGHT in 40 60 80 100 125 150 175 200 250 300 350 400 450 500
do
	../a.out -n 25 -N 300 -m 0.5 -d 0 -k 1 -W $MAX_WEIGHT -C 250 2> /dev/null > hmotnost_max_$MAX_WEIGHT
done
