#!/bin/sh

make check

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

do

    ./check 2048 4 | grep "Total time" | cut -d ' ' -f 3 >> res.txt

done

sort -n res.txt >> sortres.txt

head -5 sortres.txt | awk '{_+=$0}END{printf "Avg: %0.2f\n",_/NR}' >> out.txt

var=$(cat out.txt | grep "Avg" | cut -d ' ' -f 2)

printf "%f\n" $(bc -l <<< "42000/$var")

rm res.txt

rm sortres.txt

rm out.txt