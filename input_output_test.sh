#!/bin/bash

for i in `seq 1 100`;
do
    ./main output 1,\"abc\",3.1415,\"e\" &
	./main input i:1,s:\"abc\",f:*,s:\>\"d\" &
done    
sleep 5
rm /tmp/tuple_test & rm /tmp/proc_test & rm /tmp/lin_mes_* &