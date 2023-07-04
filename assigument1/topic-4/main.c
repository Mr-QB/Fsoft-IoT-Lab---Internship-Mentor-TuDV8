#include <stdio.h>

int isLittleEndian() {
    /*
     * Check if your computer is using Little Endian or Big Endian.
     * If the computer is little endian, the value of the first byte of an integer will be 1; otherwise, it will be 0.
     */
    int value = 1;
    char* ptr = (char*)&value;

    // If the first byte is the least significant byte, it is little endian
    if (*ptr == 1) {
        return 1;
    } else {
        return 0;
    }
}

#define BIG_TO_LITTLE_ENDIAN(value) \
    ((((value) >> 24) & 0xFF) | \
     (((value) >> 8) & 0xFF00) | \
     (((value) << 8) & 0xFF0000) | \
     (((value) << 24) & 0xFF000000))

int main() {
    // Check endianness
    if (isLittleEndian()) {
        printf("This computer is using Little Endian.\n");
    } else {
        printf("This computer is using Big Endian.\n");
    }

    // Convert value from big endian to little endian
    unsigned int value = 0x11223344;
    printf("Value Before Converting = 0x%08X\n", value);

    value = BIG_TO_LITTLE_ENDIAN(value);
    printf("Value After Converting = 0x%08X\n", value);

    return 0;
}
