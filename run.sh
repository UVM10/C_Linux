#!/bin/bash

if [ ! -f Danhsach ]; then
    echo "Can't find Danhsach!"
    exit 1
fi

./Danhsach
