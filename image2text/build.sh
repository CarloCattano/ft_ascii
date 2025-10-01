#!/bin/env bash

if [ -f ./img2txt ]; then
    rm ./img2txt
fi

cc main.c -o img2txt -lm

if [ $? -eq 0 ]; then
    printf "\nBuild successful!"
else
    printf "\nBuild failed!"
fi


printf "\nUsage: \n img2txt <image.[png/jpg]> <size> \n"
printf "\nexample:\n ./img2txt zebra.jpg 42 \n"
