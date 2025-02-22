#!/bin/bash

gcc Danhsach.c -o Danhsach 

if [ $? -eq 0 ]; then
    echo "Complete!"
else
    echo "Error!"
    exit 1
fi
