# LBYARCH-MP2: : x86-to-C Interface Programming Project

Group Members:
- Young, Cedric
- Ross, Isabella Althea R. Ross
- S18A

**PROJECT OVERVIEW
**  
This project implements a kernel to perform the DAXPY function ($Z = A \cdot X + Y$) using:

1. A C version (host and reference).
2. An x86-64 Assembly version (using scalar double-precision instructions).
The project measures and compares the execution time of the Assembly kernel across different vector sizes (2^10, 2^24, 2^29).

******
Type instructions below into CMD in the folder to run
******

nasm -f win64 daxpy.asm -o daxpy.o

gcc -c main.c -o main.o

gcc main.o daxpy.o -o run.exe

.\run.exe
