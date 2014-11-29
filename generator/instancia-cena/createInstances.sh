#!/bin/bash

for MAX_PRICE in 40 60 80 100 125 150 175 200 250 300 350 400 450 500
do
	../a.out -n 25 -N 300 -m 0.5 -d 0 -k 1 -W 250 -C $MAX_PRICE 2> /dev/null > cena_max_$MAX_PRICE
done
