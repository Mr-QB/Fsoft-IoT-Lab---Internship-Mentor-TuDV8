#include "stdio.h"
#include "string.h"


char *stringProcessing(char *string) {
    /*
     * Reverse words in a given string.
     * @param: Pointer of given string.
     * @return: Pointer of string (reverse sequence of input string word by word).
     * Example:
        ** Input string: “The quick brown fox jumps over the lazy dog!”.
        ** Output string: “dog! lazy the over jumps fox brown quick The”.
     * Algorithm: reverse the whole string then reverse each word in the string.
     */
    int length = strlen(string);
    for (int i = 0, j = length-1; i < j; i++, j--) {  // Reverse the whole string.
        char temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }

    int index = 0;
    while (index < length) { // Reverse each word in the string.
        int start = index, end = index; //  Start and end positon of word.
        while (index < length && string[index] != 32) { // Find starting positon of word.
            end++;
            index++;
        }
        for (int i = start, j = end - 1; i < j; i++, j--) { // Reverse the word.
            char temp = string[i];
            string[i] = string[j];
            string[j] = temp;
        }
        index++;
    }

    return string;
}

int main() {
    char string[] = "The quick brown fox jumps over the lazy dog!";
    printf("Input string: %s\n", string);
    stringProcessing(string);
    printf("Out string: %s\n", string);
}