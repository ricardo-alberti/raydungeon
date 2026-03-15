#!/bin/bash

rm raydungeon
cc -g -Wall code/main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o raydungeon
./raydungeon
