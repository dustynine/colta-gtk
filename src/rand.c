#include <stdio.h>

void main() {
    unsigned int seed, hexa;

    seed = 16483746;
    srand(seed);
    hexa = rand() & 0xFF;
    printf("%x\n", hexa);
}
