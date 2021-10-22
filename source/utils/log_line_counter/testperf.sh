#!/bin/bash

go build


for  (( procCount=1; procCount<=15; procCount++ ))
do

    touch testfile2.txt
    rm testfile2.txt

    touch testfile.txt

    for (( i=1; i<=procCount; i++ ))
    do
        echo "$procCount $i"
        ./log_line_counter testfile.txt &
    done

    # give all the processes a little time to start
    sleep 0.5

    start=`date +%s.%N`

    for i in {1..1000000}
    do
        echo "$i  asdfasdfasdfasdfasdfasdfasdfasdfasdfasd" >> testfile.txt
    done
    echo "eof" >> testfile.txt

    wait
    
    mv testfile.txt testfile2.txt

    end=`date +%s.%N`

    runtime=$( echo "$end - $start" | bc -l )
    echo "processes: " $procCount " runtime: " $runtime

done

exit
