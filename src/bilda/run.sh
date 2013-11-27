#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/include

if [-n $1]
then
    ./bilda -debug -dt 0 (-burn)
    ./bilda -debug -dt 1
    ./bilda -debug -dt 2 
else
    make rm
    make
    ./bilda -debug -dt 0
    ./bilda -debug -dt 1
    ./bilda -debug -dt 2
fi


