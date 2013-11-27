#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/include

if [-n $1]
then
    ./ilda -debug -dt 0 
    ./ilda -debug -dt 1
    ./ilda -debug -dt 2
else
    make rm
    make
    ./ilda -debug -dt 0
    ./ilda -debug -dt 1
    ./ilda -debug -dt 2
fi


