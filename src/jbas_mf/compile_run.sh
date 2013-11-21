#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/include:.$LD_BIRARY_PATH

if [-n $1]
then
    ./jbas_mf
else
    make rm
    make
    ./jbas_mf
fi


