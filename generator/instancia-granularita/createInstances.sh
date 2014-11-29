#!/bin/bash

for GRAN in "0.1" "0.2" "0.4" "0.6" "0.8" "1" "1.5" "2" "4" "6" "8"
do
	../a.out -n 25 -N 300 -m 0.5 -d 1 -k "$GRAN"  -W 250 -C 250 2> /dev/null > granularita_$GRAN
done
