#!/bin/bash

# does not account for neg numbers
sci_notation_regex='^[0-9]+([.][0-9]+)?(e[0-9]+|e-[0-9]+)?$'

function test_time {
    # compare 
    if [[ ! $1 =~ $sci_notation_regex ]] ; 
    then
        echo ERROR: time is not on stderr or not formatted properly
        echo
        rm .time
        exit 1
    fi
    # delete tmp file 
    rm .time
}

SUCCESS_FILE=.passed_bubblesort

# remove success file
if [ -e ${SUCCESS_FILE} ] ;
then
    rm ${SUCCESS_FILE} 
fi
echo THREAD=${THREAD}: before starting time is $(date)
for n in  10 1000 1111 1017 10000 10017;
do 
    for nbt in 1 2 3 4;
    do
        TEST=$(./bubblesort_seq $n $nbt 2> .time)
        if [ "$TEST" = "checked" ] ; 
        then
            test_time $(cat .time)
        else
            echo FAIL: "./bubblesort_seq $n $nbt is not sorted correctly"
            exit 1
        fi
    done
done
echo THREAD=${THREAD}: before starting time is $(date)

touch ${SUCCESS_FILE} 

echo "================================"
echo
echo "Success! All BUBBLESORT tests passed! :)"
