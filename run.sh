#!/usr/bin/env sh
set -e
gcc *.c -o connect4 -std=c99
./connect4
