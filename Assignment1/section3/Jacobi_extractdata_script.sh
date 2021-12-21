#!/bin/bash

# parallel execution                                        

for f in ./Data_Jacobi/* ;
do
awk 'BEGIN{i=0} {if($1==4){x+=$9;i+=1} if($4==2){y=$10}} END{printf "%.6f , %.2f , ",x/i,y;print FILENAME}' $f 1>>Jacobi_benchmark.csv
done                                        
exit
