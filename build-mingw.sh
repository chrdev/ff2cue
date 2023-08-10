#!/bin/sh
mkdir -p build
cd src
gcc -O2 -Wall -municode -mconsole -s mos.c cue.c cueconv.c ff2cue.c -o ../build/ff2cue.exe
