# LBYARCH-MP2

******
Type instructions below into CMD in the folder to run
******

nasm -f win64 daxpy.asm -o daxpy.o

gcc -c main.c -o main.o

gcc main.o daxpy.o -o run.exe

.\run.exe
