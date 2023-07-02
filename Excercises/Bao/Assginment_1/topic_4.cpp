#include "stdio.h"

#define BIG_TO_LITTLE_ENDIAN(value) (((value) >> 24) & 0xFF) | (((value) >> 8) & 0xFF00) | (((value) << 8) & 0xFF0000) | (((value) << 24) & 0xFF000000)

int isLittleEndian() {
    /*
     * Check if your computer is using Little Endian or Big Endian.
     * If the computer is little endian, the value of the first byte of an integer will be 1; otherwise, it will be 0.
     */
    unsigned int i = 1;
    char *c = (char *) &i;
    if (*c) {
        printf("System is Little endian\n");
        return 1;
    } else
        printf("System is Big endian\n");
    return 0;
}

int convert(int value) {
    value = BIG_TO_LITTLE_ENDIAN(value);
    return value;
}

int main() {
    isLittleEndian();
    unsigned int value = 0x11223344;
    printf("Value before conversion: 0x%08X\n", value);
    printf("Value after conversion: 0x%08X\n", convert(value));
    return 0;
}