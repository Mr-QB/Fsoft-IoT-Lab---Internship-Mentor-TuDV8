#include <stdio.h>
#include <string.h>

void reverseWords(char* str) {
    
    /*
     * Reverse words in a given string.
     * Example:
        ** Input string: “The quick brown fox jumps over the lazy dog!”.
        ** Output string: “dog! lazy the over jumps fox brown quick The”.
     */
    int length = strlen(str);
    int i, j;

    // Traverse the string from the end to the beginning
    for (i = length - 1; i >= 0; i--) {
        // When a space or the start of the string is encountered,
        // it indicates the end of a word
        if (str[i] == ' ' || i == 0) {
            int start = (i == 0) ? 0 : i + 1;
            int end = (i == 0) ? length - 1 : i - 1;

            // Print the word
            for (j = start; j <= end; j++) {
                printf("%c", str[j]);
            }

            // Print a space if it's not the last word
            if (i > 0) {
                printf(" ");
            }
        }
    }
}

int main() {
    char str[] = "The quick brown fox jumps over the lazy dog!";
    printf("Input string: %s\n", str);

    printf("Output string: ");
    reverseWords(str);

    return 0;
}

