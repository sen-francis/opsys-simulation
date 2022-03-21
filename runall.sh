#!/bin/sh

rm -f a.out
gcc -Wall src/*.c -lm

rm -f output/output*.txt
rm -f output/simout*.txt

# test _2
a.out 1 9 0.01 4096 4 0.5 256 > output/output02-full.txt

# test _3
a.out 2 9 0.01 4096 4 0.5 256 > output/output03-full.txt

# test _4
a.out 8 9 0.01 4096 4 0.75 256 > output/output04-full.txt

# test _5
a.out 8 101 0.001 16384 4 0.5 2048 > output/output05-full.txt


# test _1 compile (gcc; g++; javac; python -m py_compile p1.py)
gcc -Wall src/*.c -lm -D DISPLAY_MAX_T=1000

# test _2
a.out 1 9 0.01 4096 4 0.5 256 > output/output02.txt
mv simout.txt output/simout02.txt

# test _3
a.out 2 9 0.01 4096 4 0.5 256 > output/output03.txt
mv simout.txt output/simout03.txt

# test _4
a.out 8 9 0.01 4096 4 0.75 256 > output/output04.txt
mv simout.txt output/simout04.txt

# test _5
a.out 8 101 0.001 16384 4 0.5 2048 > output/output05.txt
mv simout.txt output/simout05.txt
